#ifndef GLOBAL_H
#define GLOBAL_H
#include "Engine/Logger.h"

class Global
{
public:

    static Global& instance();
    Logger& getLogger() {return *log;}
private:
    Global();
    ~Global();
    static Global *Instance;
    Logger *log;
};

#endif // GLOBAL_H
