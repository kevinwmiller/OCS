#ifndef OCS_DESTROY_OBJ_CMD_H
#define OCS_DESTROY_OBJ_CMD_H

#include <queue>
#include <string>

#include <OCS/Objects/Object.hpp>
#include <OCS/Objects/ObjectManager.hpp>

namespace ocs
{

/*! \brief Command to destroy an object
*         
*
*/
class DestroyObject : public ObjectCommand
{

public:

    DestroyObject(ObjectManager& _objManager, ID _objectId) :
        ObjectCommand(_objManager, _objectId)
    {}

    void execute() 
    {
        //std::cout << "\tExecuting DestroyObject\n";
        objManager.destroyObject(objectId);
    }

private:

};

}

#endif