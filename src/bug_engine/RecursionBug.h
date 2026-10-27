#ifndef RECURSIONBUG_H
#define RECURSIONBUG_H

#include "Bug.h"

class RecursionBug : public Bug
{
public:
    BugData generate() override;
};

#endif