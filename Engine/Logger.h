#ifndef LOGGER_H
#define LOGGER_H
#include <QString>
#include <QFile>
enum MsgType
{
    Text = 0,
    Warning,
    Error
};

class Logger
{
public:
    Logger();
    ~Logger();
    void log(MsgType type, const QString &logType, const QString &msg);
private:
    QFile logFile;
};

#endif // LOGGER_H
