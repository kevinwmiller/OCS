#ifndef OCS_RUN_SCRIPT_CMD_H
#define OCS_RUN_SCRIPT_CMD_H

namespace ocs
{

/*! \brief A command to run some Python/(Maybe Lua) script. 
*          Currently not implemented.
*
*/
class RunScript
{

public:

    virtual void execute() = 0;
    
private:
};

}

#endif