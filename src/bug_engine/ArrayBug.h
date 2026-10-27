#ifndef ARRAYBUG_H
#define ARRAYBUG_H

#include "Bug.h"

class ArrayBug : public Bug
{
public:
    BugData generate() override;
};

#endif