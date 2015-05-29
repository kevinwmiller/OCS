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

#include "OCS/Messaging/Message.hpp"
#include "OCS/Messaging/Transceiver.hpp"

struct TextMessage : public ocs::Message<TextMessage>
{
    TextMessage(const ocs::Transceiver& transceiver, std::string _msg) : Message(transceiver), msg(_msg) {}

    //Optional
    void log(std::ostream& out)
    {
        out << "Message Type: TextMessage\n";
        out << "Message Family: " << componentFamily() << std::endl;
        out << "Sender: " << getSender() << std::endl;
        out << "Message: " << msg << std::endl;
    }

    std::string msg;
};

struct Explosion : public ocs::Message<Explosion>
{
    Explosion(const ocs::Transceiver& transceiver, int _radius, int _damage) : Message(transceiver),
                        radius(_radius), damage(_damage) {}

    //Optional
    void log(std::ostream& out)
    {
        out << "Message Type: Explosion\n";
        out << "Message Family: " << componentFamily() << std::endl;
        out << "Sender: " << getSender() << std::endl;
        out << "Radius: " << radius << std::endl;
        out << "Damage: " << damage << std::endl;
    }

    int radius;
    int damage;
};
