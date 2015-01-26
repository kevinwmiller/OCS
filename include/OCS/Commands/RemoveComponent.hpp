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
*
*   Usage: 
*       ocs::RemoveComponent<Position> rem(objManager, 3, Position(5, 4));
*   This will create a RemoveComponent command object called "rem" which will
*   add a Position component initialized with the values 5 and 4 to object with id 3
*   which lives in some ObjectManager "objManager"
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
        std::cout << "Executing Remove " << componentType.getFamily() << std::endl;
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
        remComponentCommands.emplace_back(new RemoveComponent<C>(objManager, objectId));
        storeComponents<Args...>();
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
    template<typename T = SentinalType, Args...>
    void storeComponents()
    {
        std::cout << "Test " << T().getFamily() << "\n";
        if(!SentinalType::endRecursion(T()))
        {
            //Constructor for RemoveComponent will call storeComponents which will create another instance if
            //there are more components
            remComponentCommands.emplace_back(new RemoveComponent<T>(objManager, objectId));
        }
    }

    std::vector<std::unique_ptr<ObjectCommand>> remComponentCommands;
    
};

}

#endif