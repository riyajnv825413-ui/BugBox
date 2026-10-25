#ifndef COMPILERENGINE_H
#define COMPILERENGINE_H

#include <QString>
#include <QtGlobal>

class CompilerEngine
{
public:
    enum Status {
        Accepted,
        WrongAnswer,
        CompilationError,
        RuntimeError,
        TimeLimitExceeded,
        InvalidSubmission,
        InternalError
    };

    struct Result {
        Status status = InternalError;
        QString message;
        QString output;
        QString errorOutput;
        int exitCode = -1;
        qint64 executionTimeMs = 0;
    };

    explicit CompilerEngine(
        const QString &compilerPath = QStringLiteral("g++"));

    Result evaluate(
        const QString &sourceCode,
        const QString &input,
        const QString &expectedOutput,
        int timeLimitMs = 2000,
        int compileLimitMs = 10000) const;

    static QString statusName(Status status);

private:
    QString compilerPath;
};

#endif