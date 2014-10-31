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


#include <cassert>
#include <iostream>

#include <OCS/OCS.hpp>
#include <SampleSystems.hpp>

using namespace ocs;

namespace systest
{

ObjectManager objManager;
MessageHub msgHub;

SystemManager sysManager(objManager, msgHub);

void TEST_SYSTEM_VERSIONS()
{
    std::cout << "Testing System Versions\n";

    SystemManager sysManager1(objManager, msgHub);
    SystemManager sysManager2(objManager, msgHub);

    assert(sysManager.getVersion() == 0);
    assert(sysManager1.getVersion() == 1);
    assert(sysManager2.getVersion() == 2);

    {
        SystemManager sysManager3(objManager, msgHub);
        assert(sysManager3.getVersion() == 3);
    }

    SystemManager sysManager3(objManager, msgHub);
    assert(sysManager3.getVersion() == 3);

    std::cout << "Finished Testing System Versions\n";
}

void TEST_ADD_SYSTEM()
{
    std::cout << "Testing Adding Systems\n";

    sysManager.addSystem<MovementSystem>();

    assert(sysManager.getTotalSystems() == 1);
    assert(sysManager.hasSystem<MovementSystem>());

    sysManager.addSystem<MovementSystem>();
    assert(sysManager.getTotalSystems() == 1);

    sysManager.addSystem<NameDisplayer>();
    assert(sysManager.getTotalSystems() == 2);
    assert(sysManager.hasSystem<NameDisplayer>());

    SystemManager otherSystemManager(objManager, msgHub);

    otherSystemManager.addSystem<MovementSystem>();
    assert(otherSystemManager.getTotalSystems() == 1);
    assert(otherSystemManager.hasSystem<MovementSystem>());


    std::cout << "Finished Testing Adding Systems\n";
}

void TEST_REMOVE_SYSTEM()
{
    std::cout << "Testing Removing Systems\n";

    sysManager.addSystem<MovementSystem>();
    sysManager.addSystem<NameDisplayer>();

    assert(sysManager.getTotalSystems() == 2);

    sysManager.removeSystem<MovementSystem>();
    assert(sysManager.getTotalSystems() == 1);

    assert(!sysManager.hasSystem<MovementSystem>());


    sysManager.addSystem<MovementSystem>();
    assert(sysManager.hasSystem<MovementSystem>());

    std::cout << "Finished Testing Removing Systems\n";
}

}//systest

void testSystemManager()
{
    std::cout << "\nTesting Systems\n";
    systest::TEST_SYSTEM_VERSIONS();
    systest::TEST_ADD_SYSTEM();
    systest::TEST_REMOVE_SYSTEM();
    std::cout << "Finished Testing Systems\n";
}
