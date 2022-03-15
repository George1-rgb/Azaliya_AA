#include "Engine/Logger.h"
#include <QTextStream>

#include <QDebug>
#include <QDateTime>
Logger::Logger()
{
    QString fname = "D:/QTProjects/Azaliya_AA/Log/"+QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss") + ".txt";
    logFile.setFileName(fname);
    if(!logFile.open(QIODevice::WriteOnly))
    {
        qDebug() << "Cannot open log file";
        return;
    }
}

Logger::~Logger()
{
    logFile.close();
}

void Logger::log(MsgType type, const QString &logType, const QString &msg)
{

    QTextStream out(&logFile);
    switch(type)
    {
    case Text: out << "\n" << QDateTime::currentDateTime().toString("hh:mm:ss ") << "TEXT: "; break;
    case Warning: out << "\n" << QDateTime::currentDateTime().toString("hh:mm:ss ") << "WARNING: "; break;
    case Error: out << "\n" << QDateTime::currentDateTime().toString("hh:mm:ss ") << "ERROR: "; break;
    }

    out << "(" << logType << "): " << msg;

}

