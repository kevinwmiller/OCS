#ifndef OCS_COMMAND_H
#define OCS_COMMAND_H

namespace ocs
{

/*! \brief Base Command class
*          A command is an event that can be stored and executed at a later time
*          by some "Invoker" object
*
*/
class Command
{

public:

    virtual ~Command() {}
    virtual void execute() = 0;

private:
};

}

#endif