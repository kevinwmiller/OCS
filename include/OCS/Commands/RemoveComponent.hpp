#ifndef OCS_REMOVE_COMPONENT_CMD_H
#define OCS_REMOVE_COMPONENT_CMD_H

#include <iostream>
#include <memory>
#include <vector>

#include <OCS/Commands/Command.hpp>
#include <OCS/Components/Component.hpp>
#include <OCS/Objects/ObjectManager.hpp>
#include <OCS/Components/SentinalType.hpp>

namespace ocs
{

/*! \brief Command to Remove a single component to an object
*   This command is use internally by 'RemoveComponents'(Plural). 
*
*   \see RemoveComponents
*   Usage: 
*       ocs::RemoveComponent<Position> rem(objManager, 3);
*   This will create a RemoveComponent command object called "rem" which will
*   remove the Position component (If it exists. Otherwise nothing happens) from the object with id 3
*/
template<typename C>
class RemoveComponent : public ocs::ObjectCommand
{

public:

    RemoveComponent(ObjectManager& _objManager, ID _objectId) :
        ObjectCommand(_objManager, _objectId),
        componentType()
    {
    }

    void execute()
    {
        objManager.removeComponents<C>(objectId);
    }

private:

    //!Stored to keep track of the component type
    C componentType;
};

/*! \brief Command to remove multiple components to an object
*
*   This class stores a vector of unique pointers to "RemoveComponent" commands(The single version)
*
*   This command should be preferred over the singular version 'RemoveComponent' because the plural version
*   provides the same functionality in addition to the ability to add multiple
*   Usage:
*       ocs::RemoveComponents<Position, Name> rem(objManager, 0);
*       ...some time later...
*       rem.execute();       
*
*/
template<typename C, typename ... Args>
class RemoveComponents : public ocs::ObjectCommand
{

public:

    RemoveComponents(ObjectManager& _objManager, ID _objectId) :
        ObjectCommand(_objManager, _objectId)
    {
        storeComponents<C, Args...>();
    }

    void execute()
    {
        for (auto& remCmd : remComponentCommands)
        {
             //Ensure that the 'single' commands have the same objectId (If it was changed after construction)
             remCmd->setObjectId(objectId);
             remCmd->execute();
        }
    }

private:
    
    //!Create new RemoveComponent command objects until all components have been handled
    template<typename T = SentinalType, typename ... Others>
    void storeComponents()
    {
        remComponentCommands.emplace_back(new RemoveComponent<T>(objManager, objectId));
        if (sizeof ... (Others) > 0)
            storeComponents<Others...>();
    }

    std::vector<std::unique_ptr<ObjectCommand>> remComponentCommands;
    
};

}

#endif