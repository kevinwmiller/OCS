#ifndef OCS_CREATE_OBJ_CMD_H
#define OCS_CREATE_OBJ_CMD_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <OCS/Commands/Command.hpp>
#include <OCS/Commands/AssignCommand.hpp>

#include <OCS/Objects/Object.hpp>
#include <OCS/Objects/ObjectManager.hpp>

namespace ocs
{

/*! \brief Command to create an object
*
*   Objects can be created by passing any number of components to the contructor.
*   A blank object will be created if no components are given
*   Objects may also be created by giving the name of the prototype.
*   Any components passed with the prototype name will be added to the default
*   components provided by the prototype. Note that objects may only have one of each type
*   The component provided by the prototype will take precedence
*
*/
class CreateObject : public ocs::Command
{

public:

    template<typename S, typename ... Args>
    CreateObject(ObjectManager& _objManager, const S& _prototypeName, Args&& ... Components) :
        objManager(_objManager),
        prototypeName(_prototypeName),
        add(new Assign(objManager, -1, Components...))
    {
  //      add = new
    }

    template<typename ... Args>
    CreateObject(ObjectManager& _objManager, Args&& ... Components) :
        objManager(_objManager),
        //add(assign::add(objManager, -1, Components...))
        add(new Assign(objManager, -1, Components...))
    {
        //add = new Assign(objManager, -1, Components...);
    }

    ~CreateObject()
    {
        //std::cout << "\tDestroying CreateObject\n";
    }

    void execute()
    {
        //std::cout << "\tExecuting create " << prototypeName <<"\n";
        ID objectId = -1;

        if (!prototypeName.empty())
            objectId = objManager.createObject(prototypeName);
        else
            objectId = objManager.createObject();

        //std::cout << "Object Id: " << objectId << "\n";
        add->setObjectId(objectId);
        add->execute();


    }

private:

    ObjectManager& objManager;
    std::string prototypeName;
    std::unique_ptr<Assign> add;
};

}

#endif