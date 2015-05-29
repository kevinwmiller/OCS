#ifndef OCS_ADD_COMPONENT_CMD_H
#define OCS_ADD_COMPONENT_CMD_H

#include <iostream>
#include <memory>
#include <vector>

#include <OCS/Commands/ObjectCommand.hpp>
#include <OCS/Components/Component.hpp>
#include <OCS/Objects/ObjectManager.hpp>

namespace ocs
{


/*! \brief Command to add multiple components to an object
*   This class stores a vector of unique pointers to "assign" commands(The single version)
*   This class should be preferred over the other command 'AddComonent' (Singlular) because the type
*   is automatically deduced in the plural version.
*
*   If a single component is needed, you can simply pass one in to the constructor
*   Usage:
*       assign addMultiple(objManager, id2, Position(), Collidable());
*       assign addSingle(objManager, id2, Position());
*       ...some time later...
*       add.execute();
*
*/
class Assign : public ObjectCommand
{

public:

    Assign(ObjectManager& _objManager, ID _objectId) :
        ObjectCommand(_objManager, _objectId)
    {}

    template<typename C, typename ... Args>
    Assign(ObjectManager& objManager, ID objectId, const C& component, Args&& ... others) :
        ObjectCommand(objManager, objectId)
    {
        storeComponents(component, others...);
    }

    ID getNumComponents() const { return assignCommands.size(); }

    void execute()
    {
        //std::cout << "assign " << objectId << "\n";
        //int i = 0;
        for (auto& assignCmd : assignCommands)
        {
             //Ensure that the 'single' commands have the same objectId (If it was changed after construction)
            assignCmd->setObjectId(objectId);
            assignCmd->execute();
            //std::cout << "... assign " << i++ << "\n";
        }
    }

private:

    /*! \brief Command to add a single component to an object
    *   This command is used internally by the 'assign'(Plural) command
    *
    *   \see assign
    *   Usage:
    *       ocs::assign<Position> add(objManager, 3, Position(5, 4));
    *       ...some time later...
    *       add.execute();
    *   This will create an assign command object called "add" which will
    *   add a Position component initialized with the values 5 and 4 to object with id 3
    *   which lives in some ObjectManager "objManager"
    */
    template<typename C>
    class AssignComponent : public ObjectCommand
    {

    public:

        AssignComponent(ObjectManager& _objManager, ID _objectId, const C& _component) :
            ObjectCommand(_objManager, _objectId),
            component(_component)
        {
        }

        void execute()
        {
            // std::cout << "\tExecuting Add " << component.componentFamily() << " on object id " << objectId << std::endl;
            objManager.assign(objectId, component);
            //std::cout << objManager.getTotalObjects();
        }

    private:

        C component;
    };

    //!Create new assign command objects until all components have been handled
    template<typename C, typename ... Args>
    void storeComponents(const C component, Args&& ... others)
    {
        assignCommands.emplace_back(new AssignComponent<C>(objManager, objectId, component));
        if (sizeof ... (others) > 0)
            storeComponents(others...);
    }

    //! To stop recursion
    void storeComponents() {}

    std::vector<std::unique_ptr<ObjectCommand>> assignCommands;

};

}

#endif