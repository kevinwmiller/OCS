#ifndef OCS_OBJ_COMMAND_H
#define OCS_OBJ_COMMAND_H

#include <OCS/Commands/Command.hpp>
#include <OCS/Components/Component.hpp>
#include <OCS/Objects/ObjectManager.hpp>

namespace ocs
{

/*! \brief Base Command class for commands that deal with objects/components
*          Provides utilities for changing object settings after command creation
*
*/
class ObjectCommand : public Command
{

public:

    ObjectCommand(ObjectManager& _objManager, ID _objectId) :
        objManager(_objManager),
        objectId(_objectId)
    {}

    virtual ~ObjectCommand() {}

    virtual void setObjectId(ID _objectId) {objectId = _objectId;}
    ID getObjectId() const { return objectId; }
    virtual void execute() = 0;
    
protected:

    ObjectManager& objManager;
    ID objectId;
};

}

#endif