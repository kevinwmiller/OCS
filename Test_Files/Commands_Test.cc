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
void TEST_CREATE_OBJECT_COMMAND()
{
    std::cout << "Testing Create Object Command\n";

    CreateObject createCmd(objManager, Position(3, 4), Name("Test1"));
std::cout << "1\n";
    assert(objManager.getTotalObjects() == 0);
std::cout << "2\n";
    createCmd.execute();
    assert(objManager.getTotalObjects() == 1);
std::cout << "3\n";
  /*  Position* pos = objManager.getComponent<Position>(0);
    std::cout << "4\n";
    Name* name = objManager.getComponent<Name>(0);
    std::cout << "";
    assert(pos->x = 3 && pos->y ==4);
std::cout << name->name << "\n";
    assert(name == "Test1");*/
    std::cout << "Finished Testing Create Object Command\n";
}

void TEST_DESTROY_OBJECT_COMMAND()
{
    std::cout << "Testing Destroy Object Command\n";

    std::cout << "Finished Testing Destroy Object Command\n";
}

void TEST_ADD_COMPONENTS_COMMAND()
{
    std::cout << "Testing Add Components Command\n";

    std::cout << "Finished Testing Add Components Command\n";
}

void TEST_REMOVE_COMPONENTS_COMMAND()
{
    std::cout << "Testing Remove Components Command\n";

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
    cmdtest::TEST_RUN_SCRIPT_COMMAND();
    std::cout << "Finished Testing Commands\n";
}
