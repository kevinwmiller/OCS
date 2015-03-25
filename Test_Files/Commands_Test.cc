/*Copyright (c) <2014> Kevin Miller - KevM1227@gmail.com

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

   3. This notice may not be removed or altered from any source
   distribution.
*/

#include "Commands_Test.hpp"
#include <SampleSystems.hpp>
#include <SampleComponents.hpp>
#include <SampleMessages.hpp>

#include <OCS/OCS.hpp>

#include <iostream>
#include <cassert>

using namespace ocs;

namespace cmdtest
{

ObjectManager objManager;

bool operator==(const Position& lhs, const Position& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

void TEST_CREATE_OBJECT_COMMAND()
{
    std::cout << "Testing Create Object Command\n";

    std::string file("prototypes.txt");
    objManager.bindStringToComponent<Position>("Position");
    objManager.bindStringToComponent<Name>("Name");
    objManager.bindStringToComponent<Motion>("Motion");
    objManager.bindStringToComponent<Collidable>("Collidable");
    int totalPrototypes = ObjectPrototypeLoader::loadPrototypeSet(objManager, file, "Prototypes","[Prototypes]");
    assert(totalPrototypes == 4);
    assert(objManager.doesPrototypeExist("Test1"));
    
    CreateObject createCmd(objManager, Position(3, 4), Name("Test1"));

    assert(objManager.getTotalObjects() == 0);

    createCmd.execute();
    assert(objManager.getTotalObjects() == 1);

    Position* pos = objManager.getComponent<Position>(0);
    Name* name = objManager.getComponent<Name>(0);

    assert(pos->x = 3 && pos->y ==4);
    assert(name->name == "Test1");

    createCmd.execute();
    assert(objManager.getTotalObjects() == 2);

    pos = objManager.getComponent<Position>(1);
    name = objManager.getComponent<Name>(1);

    assert(pos->x = 3 && pos->y ==4);
    assert(name->name == "Test1");

    CreateObject createFromPrototype(objManager, "Test1");
    createFromPrototype.execute();
    assert(objManager.getTotalObjects() == 3);
    assert(*(objManager.getComponent<Position>(2)) == Position(5, 65));
    assert((!objManager.hasComponents<Motion, Collidable>(2)));

    CreateObject createFromPrototypeWithMore(objManager, "Test1", Collidable(), Motion());
    createFromPrototypeWithMore.execute();
    assert(objManager.getTotalObjects() == 4);
    assert((objManager.hasComponents<Motion, Collidable>(3)));

    objManager.destroyAllObjects();
    assert(objManager.getTotalObjects() == 0);
    std::cout << "Finished Testing Create Object Command\n";
}

void TEST_DESTROY_OBJECT_COMMAND()
{
    std::cout << "Testing Destroy Object Command\n";

    ID id1 = objManager.createObject(Position(2, 3), Name("Test1"));
    ID id2 = objManager.createObject(Motion(), Name("Test2"));
    ID id3 = objManager.createObject(Collidable(), Name("Test3"));
    assert(objManager.getTotalObjects() == 3);
    assert(objManager.getTotalComponents<Name>() == 3);
    assert(objManager.getComponent<Name>(id2)->name == "Test2");

    DestroyObject destr(objManager, id2);

    assert(objManager.getTotalObjects() == 3);

    destr.execute();
    assert(objManager.getTotalObjects() == 2);
    assert(objManager.getTotalComponents<Name>() == 2);

    /*destr.execute(); //Still has same object id which shouldn't exist anymore
    assert(objManager.getTotalObjects() == 2);
    assert(objManager.getTotalComponents<Name>() == 2);*/

    destr.setObjectId(id1);
    destr.execute();
    assert(objManager.getTotalObjects() == 1);
    assert(objManager.getTotalComponents<Name>() == 1);

    objManager.destroyAllObjects();

    std::cout << "Finished Testing Destroy Object Command\n";
}

void TEST_ADD_COMPONENTS_COMMAND()
{
    std::cout << "Testing Add Components Command\n";

    //Setup
    ID id1 = objManager.createObject(Position(2, 3), Name("Test1"));
    ID id2 = objManager.createObject(Motion(), Name("Test2"));
    ID id3 = objManager.createObject(Collidable(), Name("Test3"));

    assert(objManager.getTotalObjects() == 3);
    assert(objManager.getTotalComponents<Name>() == 3);
    assert(objManager.getComponent<Name>(id2)->name == "Test2");

    AddComponents addMultiple(objManager, id2, Position(), Collidable());

    assert(objManager.getTotalComponents<Position>() == 1);
    assert(objManager.getTotalComponents<Collidable>() == 1);
    assert(!objManager.getComponent<Position>(id2));
    assert(!objManager.getComponent<Collidable>(id2));

    addMultiple.execute();

    assert(objManager.getTotalComponents<Position>() == 2);
    assert(objManager.getTotalComponents<Collidable>() == 2);
    assert(objManager.getComponent<Position>(id2));
    assert(objManager.getComponent<Collidable>(id2));

    //Create a blank object and execute the component pack command on it
    ID id4 = objManager.createObject();
    assert(objManager.getTotalObjects() == 4);

    assert(objManager.getTotalComponents<Position>() == 2);
    assert(objManager.getTotalComponents<Collidable>() == 2);
    assert(!objManager.getComponent<Position>(id4));
    assert(!objManager.getComponent<Collidable>(id4));

    addMultiple.setObjectId(id4);

    addMultiple.execute();

    assert(objManager.getTotalComponents<Position>() == 3);
    assert(objManager.getTotalComponents<Collidable>() == 3);
    assert(objManager.getComponent<Position>(id4));
    assert(objManager.getComponent<Collidable>(id4));

    //Try to add to object that doesn't exist
    AddComponent<Position> add(objManager, 500, Position());
    add.execute();

    objManager.destroyAllObjects();
    std::cout << "Finished Testing Add Components Command\n";
}

void TEST_REMOVE_COMPONENTS_COMMAND()
{
    std::cout << "Testing Remove Components Command\n";

    auto id = objManager.createObject(Position(), Motion(), Name());
    assert(objManager.getTotalObjects() == 1);
    assert((objManager.hasComponents<Position, Motion, Name>(id)));

    RemoveComponents<Position> rem(objManager, 0);
    assert((objManager.hasComponents<Position, Motion, Name>(id)));

    rem.execute();
    assert((!objManager.hasComponents<Position>(id)));
    rem.execute(); //Remove twice. Shouldn't do anything

    RemoveComponents<Position, Motion, Name> rem2(objManager, 0);
    assert((!objManager.hasComponents<Position, Motion, Name>(id)));
    objManager.destroyAllObjects();
    std::cout << "Finished Testing Remove Components Command\n";
}

void TEST_RUN_SCRIPT_COMMAND()
{
    std::cout << "Testing Run Script Command\n";

    std::cout << "Finished Testing Run Script Command\n";
}


}//cmdtest

int testCommands()
{
    std::cout << "\nTesting Commands\n";
    cmdtest::TEST_CREATE_OBJECT_COMMAND();
    cmdtest::TEST_DESTROY_OBJECT_COMMAND();
    cmdtest::TEST_ADD_COMPONENTS_COMMAND();
    cmdtest::TEST_REMOVE_COMPONENTS_COMMAND();
    // cmdtest::TEST_RUN_SCRIPT_COMMAND();
    std::cout << "Finished Testing Commands\n";
}
