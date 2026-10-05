#ifndef CONDITIONBUG_H
#define CONDITIONBUG_H

#include "Bug.h"

class ConditionBug : public Bug
{
public:
    BugData generate() override;
};

#endif