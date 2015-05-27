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

#include "ObjectManager_Test.hpp"

#include <cassert>
#include <iostream>
#include <string>

#include <OCS/OCS.hpp>

#include "SampleComponents.hpp"

using namespace ocs;

namespace objtest
{

ObjectManager objManager;
ObjectManager objManager2;

bool operator==(const Position& lhs, const Position& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

void TEST_OBJECT_PROTOTYPE()
{
    std::cout << "Testing Object Prototypes\n";
    std::string file("prototypes.txt");

    std::cout << "\tBinding Components\n";
    objManager.bindStringToComponent<Position>("Position");
    objManager.bindStringToComponent<Name>("Name");
    objManager.bindStringToComponent<Motion>("Motion");
    objManager.bindStringToComponent<Collidable>("Collidable");

    std::cout << "\tLoading Prototype Sets\n";
    int totalPrototypes = ObjectPrototypeLoader::loadPrototypeSet(objManager, file, "Prototypes","[Prototypes]");
    std::cout << "\tDone Loading Prototype Sets\n";
    assert(totalPrototypes == 4);

    assert(objManager.doesPrototypeExist("Test1"));
    assert(objManager.doesPrototypeExist("Other"));

    ObjectPrototypeLoader::loadObjectPrototype(objManager, file, "Test4");
    assert(objManager.doesPrototypeExist("Test4"));

    assert(objManager.getTotalObjects() == 0);
    assert(objManager.getTotalComponents<Position>() == 0);
    assert(objManager.getTotalComponents<Motion>() == 0);
    assert(objManager.getTotalComponents<Name>() == 0);
    assert(objManager.getTotalComponents<Collidable>() == 0);

    auto id = objManager.createObject("Test1");

    assert(id == 0);

    assert(*(objManager.getComponent<Position>(id)) == Position(5, 65));

    objManager.destroyAllObjects();

    std::cout << "Finished testing object prototypes\n";
}

void TEST_OBJECT_CREATION()
{
    std::cout << "Testing creating objects\n";
    ID id = objManager.createObject();

    assert(id == 0);
    id = objManager.createObject(Position(65, 30), Name("Test"), Motion());
    assert(id == 1);
    objManager.createObject(Name(), Collidable());
    objManager.createObject(Name());
    objManager.createObject(Name());
    assert(objManager.getComponent<Name>(id)->name == "Test");

    id = objManager.createObject(Position(), Position(), Position(), Motion(), Collidable());
    assert(objManager.getTotalComponents<Position>() == 2);

    auto objects = objManager.getObjects<Collidable, Name>();

    assert((objects.size() == 1 && objects[0] == 2));

    objManager.destroyAllObjects();

    std::cout << "Finished testing creating objects\n";


}

void TEST_OBJECT_COMPONENT_SERIALIZATION()
{
    std::cout << "Testing component serialization\n";
    auto id = objManager.createObject(Name("Test1"), Position(4, 5), Motion(5, 29));

    auto comps = objManager.serializeObject(id);

    assert(comps.size() == 3);

    objManager.destroyAllObjects();

    std::cout << "Finished testing component serialization\n";
}

void TEST_OBJECT_DESTRUCTION()
{
    std::cout << "Testing destroying objects\n";
    uint64_t idx;
    idx = objManager.createObject(Name("Test1"));
    std::cout << "Created Object. New Index:" << idx << "\n";
    idx = objManager.createObject(Name("Test2"));
    std::cout << "Created Object. New Index:" << idx << "\n";
    idx = objManager.createObject(Name("Test3"));
    std::cout << "Created Object. New Index:" << idx << "\n";
    idx = objManager.createObject(Name("Test4"));
    std::cout << "Created Object. New Index:" << idx << "\n";
    idx = objManager.createObject(Name("Test5"));
    std::cout << "Created Object. New Index:" << idx << "\n";

    objManager.destroyObject(2);
    std::cout << "Destroyed Objects 2\n";
    assert(objManager.getTotalObjects() == 4);

    assert(objManager.getComponent<Name>(4)->name == "Test5");

    ID id = objManager.createObject(Name("Test6"));

    assert(id == 2);
    assert(objManager.getComponent<Name>(2)->name == "Test6");

    objManager.destroyAllObjects();

    std::cout << "Finished testing destroying objects\n";
}

void TEST_COMPONENT_ASSIGNING()
{

    std::cout << "Testing assigning components\n";
    ID id = objManager.createObject();
    ID id2 = objManager2.createObject(Position(560, -4541), Collidable(), Name("Testing2"));

    objManager.addComponents(id, Position(56, -45), Motion(), Name("Testing"));

    objManager2.addComponents(id2, Position(560, -4541), Collidable(), Name("Testing2"));

    bool test = objManager.hasComponents<Position, Motion, Name>(id);
    assert( test );

    bool test2 = objManager2.hasComponents<Position, Collidable, Name>(id2);
    assert(test2);

    assert(objManager.hasComponents<Motion>(id));
    assert(!objManager.hasComponents<Collidable>(id));

    assert(*(objManager.getComponent<Position>(id)) == Position(56, -45));

    assert(objManager.getComponentArray<Position>()[0].getOwnerID() == id);

    assert(objManager.getComponent<Name>(id)->name == "Testing");

    assert(objManager2.getComponent<Name>(id2)->name == "Testing2");

    //Test adding a component the object already has
    objManager.addComponents(id, Position());
    assert(objManager.getComponentArray<Position>().size() == 1);

    objManager.destroyAllObjects();
    objManager2.destroyAllObjects();
    std::cout << "Finished testing assigning components\n";

}

void TEST_COMPONENT_MODIFYING()
{
    std::cout << "Testing modifying components\n";
    ID id = objManager.createObject();
    objManager.addComponents(id, Position(56, -45), Motion(), Name("Testing"));

    assert(*(objManager.getComponent<Position>(id)) == Position(56, -45));

    auto pos = objManager.getComponent<Position>(id);
    pos->x = 65;
    pos->y = 0;

    assert(*(objManager.getComponent<Position>(id)) == Position(65, 0));

    auto comp = objManager.getComponentArray<Position>()[0];

    assert(comp.x == 65 && comp.y == 0);


    objManager.destroyAllObjects();

    std::cout << "Finished testing modifying components\n";

}

void TEST_COMPONENT_REMOVING()
{
    std::cout << "Testing removing components\n";
    ID id = objManager.createObject();
    objManager.addComponents(id, Position(56, -45), Motion(), Name("Testing"));

    assert(objManager.hasComponents<Position>(id));
    assert(objManager.getComponentArray<Position>().size() == 1);

    objManager.removeComponents<Position, Motion>(id);
    objManager.removeComponents<Position>(id);

    assert(objManager.getComponentArray<Position>().size() == 0);

    bool hasComps = objManager.hasComponents<Position, Motion>(id);
    assert(!hasComps);

    objManager.addComponents(id, Position());
    assert(objManager.hasComponents<Position>(id));
    assert(objManager.getComponentArray<Position>().size() == 1);

    objManager.destroyAllObjects();
    std::cout << "Finished testing removing components\n";
}

void TEST_PACKEDARRAY()
{
    std::cout << "Testing Packed Array\n";

    PackedArray<Index> arry = {2, 4, 6, 8, 10};
    
    std::cout << arry << "\n";

    // std::cout << "At arry 1 " << arry[1] << "\n";
    // std::cout << "At arry 4 " << arry[4] << "\n";
    arry.remove(1);

    // std::cout << "After removing 1 1 valid? " << arry.isValid(1) << "\n";
    // std::cout << "After removing 1 4 valid? " << arry.isValid(4) << "\n";
    // std::cout << "At arry 4 " << arry[4] << "\n";

    std::cout << arry << "\n";


    arry.remove(2);

    // std::cout << "After removing 2 2 valid? " << arry.isValid(2) << "\n";
    // std::cout << "After removing 2 3 valid? " << arry.isValid(3) << "\n";
    // std::cout << "At arry 3 " << arry[3] << "\n";

    std::cout << arry << "\n";

    //uint64_t idx = arry.addItem(15);

    // std::cout << "New index " << idx << "\n";

    // std::cout << "After adding 15\n";
    // std::cout << "After adding 2 valid? " << arry.isValid(2) << "\n";
    std::cout << arry << "\n";

    //idx = arry.addItem(73);

    //std::cout << "New index " << idx << "\n";

    //std::cout << "After adding 73\n";
    //std::cout << "After adding 1 valid? " << arry.isValid(1) << "\n";
    std::cout << arry << "\n";

    arry.remove(4);
    std::cout << arry << "\n";
    // std::cout << "After removing 4 4 valid? " << arry.isValid(4) << "\n";
    // std::cout << "After removing 4 1 valid? " << arry.isValid(1) << "\n";
    // std::cout << "At arry 1 " << arry[1] << "\n";

    uint64_t newIdx = arry.addItem(15);
    arry.addItem(72);
    arry.addItem(176);
    std::cout << "Added " << newIdx << "\n";
    std::cout << arry << "\n";

    arry.remove(newIdx);
    std::cout << arry << "\n";


    std::cout << "Removing everything\n";

    while (arry.size() > 0) {
        std::cout << arry << "\n";
        arry.remove(arry.getId(0));
    }

    //std::cout << (int)(Index(-1)) << "\n";

    std::cout << "Done Testing Packed Array\n";
}

}//objtest

int testObjectManager()
{
    std::cout << "\nTesting ObjectManager\n";
    objtest::TEST_OBJECT_PROTOTYPE();
    objtest::TEST_OBJECT_CREATION();
    objtest::TEST_OBJECT_COMPONENT_SERIALIZATION();
    objtest::TEST_COMPONENT_ASSIGNING();
    objtest::TEST_COMPONENT_MODIFYING();
    objtest::TEST_COMPONENT_REMOVING();
    objtest::TEST_OBJECT_DESTRUCTION();
    objtest::TEST_PACKEDARRAY();
    std::cout << "Finished testing ObjectManager\n";

    return 0;
}





