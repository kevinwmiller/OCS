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


/*! \brief Command to add a single component to an object
*
*   Usage: 
*       ocs::AddComponent<Position> add(objManager, 3, Position(5, 4));
*       ...some time later...
*       add.execute();
*   This will create an AddComponent command object called "add" which will
*   add a Position component initialized with the values 5 and 4 to object with id 3
*   which lives in some ObjectManager "objManager"
*/
template<typename C>
class AddComponent : public ocs::ObjectCommand
{

public:

    AddComponent(ObjectManager& _objManager, ID _objectId, const C& _component) :
        ObjectCommand(_objManager, _objectId),
        component(_component)
    {
    }
    void setObjectId(ID _objectId) {objectId = _objectId;}
    void execute()
    {
        std::cout << "\tExecuting Add " << component.getFamily() << std::endl;
        objManager.addComponents(objectId, component);
    }

private:

    AddComponent(ObjectManager& _objManager, ID _objectId) :
        ObjectCommand(_objManager, _objectId),
        component()
    {}

    C component;
};

/*! \brief Command to add multiple components to an object
*   Due to limitations of template class type deduction, access to this class is granted through a static
*   method that returns an instance of the class
*   This class stores a vector of unique pointers to "AddComponent" commands(The single version)
*
*   Usage:
*       ocs::AddComponents add = ocs::AddComponents::add(objManager, 0, Position(5, 4), Motion(), Name("Whatever"));
*       ...some time later...
*       add.execute();
*
*/
class AddComponents : public ocs::ObjectCommand
{

public:

    AddComponents(ObjectManager& _objManager, ID _objectId) :
        ObjectCommand(_objManager, _objectId)
    {}

    template<typename C, typename ... Args>
    AddComponents (ObjectManager& objManager, ID objectId, const C& component, Args&& ... others) :
        ObjectCommand(objManager, objectId)
    {
        storeComponents(component, others...);
    }

    //!This is here because of the implementation of CreateObject to allow for creation of blank objects
    static AddComponents add(ObjectManager& objManager, ID objectId) 
    {
        return AddComponents (objManager, objectId);
    }


    template<typename C, typename ... Args>
    static AddComponents add(ObjectManager& objManager, ID objectId, const C& component, Args&& ... others)
    {
        AddComponents add(objManager, objectId);
        add.storeComponents(component, others...);
        return add;
    }

    ID getNumComponents() const { return addComponentCommands.size(); }

    void setObjectId(ID _objectId) {objectId = _objectId;}
    void execute()
    {
        std::cout << "AddComponents " << objectId << "\n";
        for (auto& addCmd : addComponentCommands)
        {
             //Ensure that the 'single' commands have the same objectId (If it was changed after construction)
             addCmd->setObjectId(objectId);
             addCmd->execute();
        }
    }

private:
    
    //!Create new AddComponent command objects until all components have been handled
    template<typename C, typename ... Args>
    void storeComponents(const C component, Args&& ... others)
    {
        addComponentCommands.emplace_back(new AddComponent<C>(objManager, objectId, component));
        if (sizeof ... (others) > 0)
            storeComponents(others...);
    }

    //! To stop recursion
    void storeComponents() {}

    std::vector<std::unique_ptr<ObjectCommand>> addComponentCommands;
    
};

}

#endif