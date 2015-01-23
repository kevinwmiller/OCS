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

#ifndef _COMPONENTS_H
#define _COMPONENTS_H

#include <sstream>
#include <string>

#include "OCS/Components/Component.hpp"
#include <iostream>

struct Position : public ocs::Component<Position>
{
    Position(float _x = 0.0f, float _y = 0.0f) : x(_x), y(_y) {}

    std::string serialize() { return serializer.serialize("Position % %", x, y); }
    void deSerialize(const std::string& str) { serializer.deSerialize("% %", str, x, y); }

    float x, y;
};


struct Motion : public ocs::Component<Motion>
{
    Motion (float _speed = 0, float _angle = 0) : speed(_speed), angle(_angle) {}

    std::string serialize() { return serializer.serialize("Motion % %", speed, angle);}
    void deSerialize(const std::string& str) { serializer.deSerialize("% %", str, speed, angle); }

    float speed, angle;
};

struct Name : public ocs::Component<Name>
{
    Name(const std::string& _name = "") : name(_name) {}

    std::string serialize() { return "Name " + name; }
    void deSerialize(const std::string& str) { name = str; }

    std::string name;
};

struct Collidable : public ocs::Component<Collidable>
{
    Collidable(float _top = 0.0f, float _left = 0.0f, float _width = 0.0f, float _height = 0.0f) :
        top(_top), left(_left), width(_width), height(_height) {}

    std::string serialize() { return serializer.serialize("Collidable % % % %", top, left, width, height); }
    void deSerialize(const std::string& str) { serializer.deSerialize("% % % %", str, top, left, width, height); }

    float top, left, width, height;
};


#endif
