#ifndef BUG_H
#define BUG_H

#include "BugData.h"

class Bug
{
public:
    virtual BugData generate() = 0;

    virtual ~Bug() = default;
};

#endif