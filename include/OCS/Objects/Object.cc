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

#include "OCS/Objects/Object.hpp"

namespace ocs
{

std::vector<std::string> Object::serializeComponents() const
{
    std::vector<std::string> serializedComponents;

    for(const auto& compIndex : componentIndices)
    {
        try
        {
            auto component = &componentArrays.at(compIndex.first)->getBaseComponent(compIndex.second);
            serializedComponents.push_back(component->serialize());
        }
        catch(const std::out_of_range& oor)
        {
            std::cerr << "Out of Range error: " << oor.what() << '\n';
        }
    }

    return serializedComponents;
}

void Object::deSerializeComponents(std::vector<std::pair<ID, std::string>> compArgs)
{
    for(const auto& compArg : compArgs)
    {
        auto compID = compArg.first;
        auto compStr = compArg.second;

        if(componentArrays.find(compID) != componentArrays.end())
        {
            auto component = &componentArrays.at(compID)->getBaseComponent(componentIndices.at(compID));
            component->deSerialize(compStr);
        }
    }

}

} //ocs

