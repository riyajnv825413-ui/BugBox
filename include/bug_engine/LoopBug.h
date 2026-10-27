#ifndef LOOPBUG_H
#define LOOPBUG_H

#include "Bug.h"

class LoopBug : public Bug
{
public:
    BugData generate() override;
};

#endif