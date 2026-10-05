#ifndef LOGICBUG_H
#define LOGICBUG_H

#include "Bug.h"

class LogicBug : public Bug
{
public:
    BugData generate() override;
};

#endif