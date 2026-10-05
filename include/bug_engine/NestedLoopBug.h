#ifndef NESTEDLOOPBUG_H
#define NESTEDLOOPBUG_H

#include "Bug.h"

class NestedLoopBug : public Bug
{
public:
    BugData generate() override;
};

#endif