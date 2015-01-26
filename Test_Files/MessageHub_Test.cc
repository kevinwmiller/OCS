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

#include <SampleSystems.hpp>
#include <SampleComponents.hpp>
#include <SampleMessages.hpp>

#include <OCS/OCS.hpp>

#include <iostream>
#include <cassert>

using namespace ocs;

namespace msgtest
{

MessageHub msgHub;
Transceiver t1;
Transceiver t2;
Transceiver t3;
Transceiver t4;

void TEST_MESSAGE_POSTING()
{
    std::cout << "Testing Message Posting\n";
    msgHub.postMessage<TextMessage>(t1, "Hello\n");
    msgHub.postMessage<TextMessage>(t2, "Hi!\n");

    auto txtMsgs = msgHub.readPostedMessages<TextMessage>();

    assert(txtMsgs[0]->getSender() == 0);
    assert(txtMsgs[0]->msg == "Hello\n");

    assert(txtMsgs[1]->getSender() == 1);
    assert(txtMsgs[1]->msg == "Hi!\n");

    std::cout << "Finished Testing Message Posting\n";
}

void TEST_MESSAGE_PM()
{
    std::cout << "Testing Private Messaging\n";

    msgHub.sendPrivateMessage<TextMessage>(1, t1, "Hello t2!\n");

    auto privateTxtMsgs = msgHub.readPrivateMessages<TextMessage>(t2);

    assert(privateTxtMsgs.size() == 1);
    assert(privateTxtMsgs[0]->msg == "Hello t2!\n");

    msgHub.clearPrivateMessages(t2);

    privateTxtMsgs = msgHub.readPrivateMessages<TextMessage>(t2);

    assert(privateTxtMsgs.size() == 0);

    std::cout << "Finished Testing Private Messaging\n";
}



}//msgtest

int testMessageHub()
{
    std::cout << "\nTesting Messaging\n";
    msgtest::TEST_MESSAGE_POSTING();
    msgtest::TEST_MESSAGE_PM();
    std::cout << "Finished Testing Messaging\n";

    return 0;
}
