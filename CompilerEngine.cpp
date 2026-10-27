#include "CompilerEngine.h"

#include <QProcess>
#include <QTemporaryDir>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QElapsedTimer>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QStringList>

namespace {

// Normalise program output for comparison:
//  - unify line endings
//  - remove leading/trailing blank lines
//  - remove trailing spaces/tabs on each line
// This accepts harmless whitespace differences without accepting a
// genuinely different answer.
QString normaliseOutput(const QString &text)
{
    QString unified = text;
    unified.replace(QStringLiteral("\r\n"), QStringLiteral("\n"));
    unified.replace(QLatin1Char('\r'), QLatin1Char('\n'));

    QStringList lines = unified.split(QLatin1Char('\n'));

    while (!lines.isEmpty() && lines.first().trimmed().isEmpty())
        lines.removeFirst();
    while (!lines.isEmpty() && lines.last().trimmed().isEmpty())
        lines.removeLast();

    for (QString &line : lines) {
        while (!line.isEmpty() &&
               (line.endsWith(QLatin1Char(' ')) || line.endsWith(QLatin1Char('\t')))) {
            line.chop(1);
        }
    }

    return lines.join(QLatin1Char('\n'));
}

} // namespace

CompilerEngine::CompilerEngine(const QString &compilerPath)
    : compilerPath(resolveCompilerPath(compilerPath))
{
}

// -----------------------------------------------------------------------
// resolveCompilerPath
// Finds g++ even when the app was launched from Explorer/Qt Creator
// without the MinGW bin directory on PATH.
// -----------------------------------------------------------------------
QString CompilerEngine::resolveCompilerPath(const QString &requested)
{
    // 1. Explicit override: BUGBOX_COMPILER=C:\Qt\Tools\mingw...\bin\g++.exe
    const QString envPath = qEnvironmentVariable("BUGBOX_COMPILER");
    if (!envPath.isEmpty() && QFileInfo(envPath).isFile())
        return QFileInfo(envPath).absoluteFilePath();

    // 2. A path supplied directly to the constructor.
    if (requested.contains(QLatin1Char('/')) || requested.contains(QLatin1Char('\\'))) {
        const QFileInfo info(requested);
        if (info.isFile())
            return info.absoluteFilePath();
    }

    // 3. Normal PATH lookup (works when launched by Qt Creator).
    const QString onPath = QStandardPaths::findExecutable(requested);
    if (!onPath.isEmpty())
        return onPath;

    // 4. Common Qt / MinGW / Code::Blocks installation locations.
    QStringList candidates;

    // Bundled next to the application (optional portable deployment).
    const QDir appDir(QCoreApplication::applicationDirPath());
    candidates << appDir.filePath(QStringLiteral("g++.exe"));
    candidates << appDir.filePath(QStringLiteral("compiler/g++.exe"));
    candidates << appDir.filePath(QStringLiteral("mingw/bin/g++.exe"));

    // Qt Maintenance Tool toolchains: C:\Qt\Tools\mingw1310_64\bin\g++.exe
    const QDir qtTools(QStringLiteral("C:/Qt/Tools"));
    if (qtTools.exists()) {
        const QStringList mingwDirs =
            qtTools.entryList(QStringList() << QStringLiteral("mingw*"),
                              QDir::Dirs | QDir::NoDotAndDotDot);
        for (const QString &dir : mingwDirs)
            candidates << qtTools.filePath(dir + QStringLiteral("/bin/g++.exe"));
    }

    // A few standard standalone installations.
    candidates << QStringLiteral("C:/msys64/ucrt64/bin/g++.exe");
    candidates << QStringLiteral("C:/msys64/mingw64/bin/g++.exe");
    candidates << QStringLiteral("C:/MinGW/bin/g++.exe");
    candidates << QStringLiteral("C:/mingw64/bin/g++.exe");
    candidates << QStringLiteral("C:/Program Files/CodeBlocks/MinGW/bin/g++.exe");
    candidates << QStringLiteral("C:/Program Files (x86)/CodeBlocks/MinGW/bin/g++.exe");

    for (const QString &candidate : candidates) {
        if (QFileInfo(candidate).isFile())
            return QDir::toNativeSeparators(candidate);
    }

    return requested;   // not found; isAvailable() will report false
}

bool CompilerEngine::isAvailable() const
{
    const QFileInfo info(compilerPath);
    return info.exists() && info.isFile();
}

QString CompilerEngine::statusName(Status status)
{
    switch (status) {
    case Accepted:           return "Accepted";
    case WrongAnswer:        return "Wrong Answer";
    case CompilationError:   return "Compilation Error";
    case RuntimeError:       return "Runtime Error";
    case TimeLimitExceeded:  return "Time Limit Exceeded";
    case InvalidSubmission:  return "Invalid Submission";
    case InternalError:      return "Internal Error";
    }

    return "Internal Error";
}

CompilerEngine::Result CompilerEngine::evaluate(
    const QString &sourceCode,
    const QString &input,
    const QString &expectedOutput,
    int timeLimitMs,
    int compileLimitMs) const
{
    Result result;

    if (sourceCode.trimmed().isEmpty()) {
        result.status = InvalidSubmission;
        result.message = "The submission is empty.";
        return result;
    }

    if (!isAvailable()) {
        result.status = InternalError;
        result.message =
            "C++ compiler not found. Install MinGW g++ (Qt Tools) or set "
            "the BUGBOX_COMPILER environment variable to the full path of g++.exe.";
        return result;
    }

    // Create a temporary working directory.
    QTemporaryDir tempDir;

    if (!tempDir.isValid()) {
        result.status = InternalError;
        result.message = "Could not create temporary directory.";
        return result;
    }

    const QString sourcePath = tempDir.filePath(QStringLiteral("main.cpp"));
    const QString executablePath = tempDir.filePath(QStringLiteral("program.exe"));

    // Save submitted source code.
    QFile sourceFile(sourcePath);

    if (!sourceFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        result.status = InternalError;
        result.message = "Could not save source code.";
        return result;
    }

    sourceFile.write(sourceCode.toUtf8());
    sourceFile.close();

    // Compile with MinGW g++.  Static libgcc/libstdc++ make the produced
    // executable runnable even when MinGW's bin folder is not on PATH.
    QProcess compiler;
    compiler.setWorkingDirectory(tempDir.path());

    QStringList compileArgs;
    compileArgs << QStringLiteral("-std=c++17")
               << QStringLiteral("-O0")
               << QStringLiteral("-fno-diagnostics-color")
               << sourcePath
               << QStringLiteral("-o")
               << executablePath
               << QStringLiteral("-static-libgcc")
               << QStringLiteral("-static-libstdc++");

    compiler.start(compilerPath, compileArgs);

    if (!compiler.waitForStarted(3000)) {
        result.status = InternalError;
        result.message = "Could not start the C++ compiler (" + compilerPath +
                         "): " + compiler.errorString();
        return result;
    }

    QElapsedTimer compileTimer;
    compileTimer.start();

    if (!compiler.waitForFinished(compileLimitMs)) {
        compiler.kill();
        compiler.waitForFinished();

        result.status = CompilationError;
        result.message = "Compilation timed out.";
        result.errorOutput = QString::fromLocal8Bit(compiler.readAllStandardError());
        return result;
    }

    result.errorOutput = QString::fromLocal8Bit(compiler.readAllStandardError());

    if (compiler.exitStatus() != QProcess::NormalExit || compiler.exitCode() != 0) {
        result.status = CompilationError;
        result.message = "Compilation failed.";
        result.exitCode = compiler.exitCode();
        return result;
    }

    // Run the compiled program.
    QProcess program;
    program.setWorkingDirectory(tempDir.path());
    program.start(executablePath);

    if (!program.waitForStarted(3000)) {
        result.status = InternalError;
        result.message = "Could not start the compiled program: " +
                         program.errorString();
        return result;
    }

    // Provide program input.
    program.write(input.toUtf8());
    program.closeWriteChannel();

    QElapsedTimer runTimer;
    runTimer.start();

    if (!program.waitForFinished(timeLimitMs)) {
        program.kill();
        program.waitForFinished();

        result.status = TimeLimitExceeded;
        result.message = "Program exceeded the time limit.";
        result.executionTimeMs = runTimer.elapsed();
        result.output = QString::fromLocal8Bit(program.readAllStandardOutput());
        result.errorOutput += QString::fromLocal8Bit(program.readAllStandardError());
        return result;
    }

    result.executionTimeMs = runTimer.elapsed();
    result.output = QString::fromLocal8Bit(program.readAllStandardOutput());
    result.errorOutput += QString::fromLocal8Bit(program.readAllStandardError());
    result.exitCode = program.exitCode();

    // Check whether the program crashed.
    if (program.exitStatus() != QProcess::NormalExit || program.exitCode() != 0) {
        result.status = RuntimeError;
        result.message = "Program crashed or returned a non-zero exit code.";
        return result;
    }

    // Compare output with the expected answer.
    const QString actual = normaliseOutput(result.output);
    const QString expected = normaliseOutput(expectedOutput);

    if (actual == expected) {
        result.status = Accepted;
        result.message = "Program executed successfully.";
    } else {
        result.status = WrongAnswer;
        result.message = "Output does not match the expected answer.";
    }

    return result;
}
