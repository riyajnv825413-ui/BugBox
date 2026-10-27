#ifndef SYNTAXBUG_H
#define SYNTAXBUG_H

#include "Bug.h"

class SyntaxBug : public Bug
{
public:
    BugData generate() override;
};

#endif