#include "CompilerEngine.h"

#include <QProcess>
#include <QTemporaryDir>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QElapsedTimer>
#include <QRegularExpression>

CompilerEngine::CompilerEngine(const QString &compilerPath)
    : compilerPath(compilerPath)
{
}

QString CompilerEngine::statusName(Status status)
{
    switch (status) {
    case Accepted:           return "Accepted";
    case WrongAnswer:       return "Wrong Answer";
    case CompilationError:  return "Compilation Error";
    case RuntimeError:      return "Runtime Error";
    case TimeLimitExceeded:return "Time Limit Exceeded";
    case InvalidSubmission:return "Invalid Submission";
    case InternalError:     return "Internal Error";
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

    // Create a temporary working directory.
    QTemporaryDir tempDir;

    if (!tempDir.isValid()) {
        result.status = InternalError;
        result.message = "Could not create temporary directory.";
        return result;
    }

    QString sourcePath =
        tempDir.filePath("main.cpp");

    QString executablePath =
        tempDir.filePath("program.exe");

    // Save submitted source code.
    QFile sourceFile(sourcePath);

    if (!sourceFile.open(QIODevice::WriteOnly |
                         QIODevice::Text)) {
        result.status = InternalError;
        result.message = "Could not save source code.";
        return result;
    }

    sourceFile.write(sourceCode.toUtf8());
    sourceFile.close();

    // Compile using MinGW g++.
    QProcess compiler;

    compiler.setWorkingDirectory(tempDir.path());

    compiler.start(
        compilerPath,
        QStringList()
            << "-std=c++17"
            << sourcePath
            << "-o"
            << executablePath
        );

    if (!compiler.waitForStarted(3000)) {
        result.status = InternalError;
        result.message = "Could not start MinGW compiler. Check compiler path.";
        return result;
    }

    QElapsedTimer compileTimer;
    compileTimer.start();

    if (!compiler.waitForFinished(compileLimitMs)) {
        compiler.kill();
        compiler.waitForFinished();

        result.status = CompilationError;
        result.message = "Compilation timed out.";
        result.errorOutput = compiler.readAllStandardError();
        return result;
    }

    result.errorOutput =
        QString::fromLocal8Bit(compiler.readAllStandardError());

    if (compiler.exitStatus() != QProcess::NormalExit ||
        compiler.exitCode() != 0) {
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
        result.message = "Could not start compiled program.";
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
        result.message = "Program exceeded time limit.";
        result.executionTimeMs = runTimer.elapsed();
        return result;
    }

    result.executionTimeMs = runTimer.elapsed();

    result.output =
        QString::fromLocal8Bit(program.readAllStandardOutput());

    result.errorOutput +=
        QString::fromLocal8Bit(program.readAllStandardError());

    result.exitCode = program.exitCode();

    // Check whether program crashed.
    if (program.exitStatus() != QProcess::NormalExit ||
        program.exitCode() != 0) {
        result.status = RuntimeError;
        result.message = "Program crashed or returned a non-zero exit code.";
        return result;
    }

    // Compare output with expected answer.
    QString actual = result.output;
    QString expected = expectedOutput;

    actual.replace("\r\n","\n");
    expected.replace("\r\n","\n");

    actual = actual.trimmed();
    expected = expected.trimmed();

    if (actual == expected) {
        result.status = Accepted;
        result.message = "Program executed successfully.";
    } else {
        result.status = WrongAnswer;
        result.message = "Output does not match expected answer.";
    }

    return result;
}