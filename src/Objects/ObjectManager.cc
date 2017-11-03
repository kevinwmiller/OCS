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

#include "OCS/Objects/ObjectManager.hpp"
#include <set>

namespace ocs
{

ID ObjectManager::prototypeIDCounter = 0;
ID ObjectManager::versionCounter = 0;
std::queue<ID> ObjectManager::availableVersions;

ObjectManager::ObjectManager()
{
    if(availableVersions.empty())
    {
        version = versionCounter++;
    }
    else
    {
        version = availableVersions.front();
        availableVersions.pop();
    }
}

ObjectManager::~ObjectManager()
{
    //destroyAllObjects();
    availableVersions.push(version);
}

/** \brief Create an object modeled after an existing object
 *         
 *
 * \param destinationId The id of the object to copy to
 * \param source The object to copy from
 */
void ObjectManager::copyObject(ID destinationId, const Object& source)
{

    //Only copy to an object that was created using the ObjectManager
    if (objects.isValid(destinationId))
    {
        //Clear the destination's components so they aren't still around after the object gets new components
        removeAllComponents(destinationId);

        //Copy the component families from the source
        for(auto& it : source.componentArrays)
            objects[destinationId].componentArrays[it.first] = compFamilyToCompArray[it.first];

        auto& sourceCompArrays = source.componentArrays;
        auto& componentIndices = objects[destinationId].componentIndices;

        //Iterate through all of the objects component array pointers
        for (const auto& it : sourceCompArrays)
        {
            Family compFamily = it.first;
            auto componentArray = objects[destinationId].componentArrays[it.first];

            auto sourceComponentArray = it.second;

            //Get the index of the source's component
            ID sourceCompIdx = source.componentIndices.at(compFamily);

            //Copy the source's component and store the index that it was assigned
            ID newComponentIndex = sourceComponentArray->createCopy(sourceCompIdx, componentArray);

            //Make the new object the component's owner
            componentArray->getBaseComponent(newComponentIndex).ownerID = destinationId;

            //Store the component's index in the map under the newly created object's ID
            componentIndices[compFamily] = newComponentIndex;

        }
    }
   
}

/** \brief Create a blank object
 *
 * \return The id of the created object
 *
 */
ID ObjectManager::createObject()
{
    ID indx = objects.emplace_item();
    objects[indx].objectID = indx;

    return indx;
}

/** \brief Create an object modeled after the prototype of the given name.
 *         If the prototype does not exist, no object is created.
 *
 * \param prototypeName The name of the prototype to base the new object off of.
 *
 * \return If the prototype exists, returns the id of the created object.
 *         If the prototype does not exist, returns -1 converted to an unsigned number
 *
 */
ID ObjectManager::createObject(const std::string& prototypeName)
{

    if(doesPrototypeExist(prototypeName))
    {
        auto& prototype = objectPrototypes[prototypeName];
        ID objectID = createObject();

        copyObject(objectID, prototype);

        return objectID;
    }

    std::cerr << "Error: No prototype exists under the name '" << prototypeName << "'\n";
    return -1;
}

/** \brief Removes an object and all of its components from the appropriate arrays.
 *
 * \param objectID The id of the object to be removed.
 *
 */

void ObjectManager::destroyObject(ID objectID)
{
    if(objects.isValid(objectID))
    {
        removeAllComponents(objectID);
        //Remove the object from the object array
        objects.remove(objectID);
    }else
        std::cerr << "Error: Invalid object id\n";
}

/** \brief Destroys all objects by iterating through the objects list and calling destroyObject.
 *
 *  \see destroyObject
 *
 */
void ObjectManager::destroyAllObjects()
{
    while (objects.size() > 0)
        destroyObject(objects[0].objectID);

    objects.clear();
}

/** \brief Gets the total number of existing objects.
 *
 *  \return The size of the array containing all objects.
 */
ID ObjectManager::getTotalObjects() const
{
    return objects.size();
}

//!Serialize all components of an object
std::vector<std::string> ObjectManager::serializeObject(ID objectID)
{
    if(objects.isValid(objectID))
    {
        return objects[objectID].serializeComponents();
    }
    //Return an empty vector if the object id was invalid
    return std::vector<std::string>();
}

void ObjectManager::deSerializeObject(ID objectID, std::vector<std::pair<Family, std::string>>& objectArgs)
{
    if(objects.isValid(objectID))
    {
        objects[objectID].deSerializeComponents(objectArgs);
    }
}

/** \brief Removes all components from an object
 *
 *  \param objectID The id of the object to remove the components from
 *  \return The number of components removed
 */
ID ObjectManager::removeAllComponents(ID objectID)
{
    ID componentsRemoved = 0;
    if(objects.isValid(objectID))
    {
        auto& componentArrays = objects[objectID].componentArrays;

        //Iterate through all of the objects component array pointers
        for(const auto& it : componentArrays)
        {
            Family compFamily = it.first;
            auto componentArray = it.second;

            //Get the component's index
            ID compIndex = objects[objectID].componentIndices[compFamily];

            //Remove the component from the appropriate array
            componentArray->remove(compIndex);
            ++componentsRemoved;
        }
    }

    return componentsRemoved;
}

/** \brief Searches the prototype map for the given prototype name to see if the prototype exists.
 *
 * \param prototypeName The name of the prototype to search for.
 *
 * \return True if the prototype exists. False if it does not.
 *
 */
bool ObjectManager::doesPrototypeExist(const std::string& prototypeName) const
{
    return objectPrototypes.find(prototypeName) != objectPrototypes.end();
}

/** \brief Updates a component from a string to deserialize using the component's bound string name
 *
 * \param objectID The ID of the object's component to update.
 * \param compName The bound string name of the component type to update.
 * \param compStr The serialized component data to deserialize.
 *
 * \return True if the update was successful. False if it fails.
 *
 */
bool ObjectManager::updateComponentFromString(ID objectID, const std::string& compName, const std::string& compStr)
{
    bool status = false;

    // Check if the object ID is valid
    if(objects.isValid(objectID))
    {
        // Get the component family from the string
        auto found = stringToCompFamily.find(compName);
        if(found != stringToCompFamily.end())
        {
            auto compFamily = found->second;

            // Get the object
            auto& obj = objects[objectID];

            // Check if the component already exists
            auto foundCompArray = obj.componentArrays.find(compFamily);
            if(foundCompArray != obj.componentArrays.end())
            {
                // Update the component if it does exist
                auto compIdx = obj.componentIndices[compFamily];
                auto component = &foundCompArray->second->getBaseComponent(compIdx);
                component->deSerialize(compStr);
                status = true;
            }
            else
            {
                // Add a new component if it doesn't exist
                // Get the component array from the component family
                foundCompArray = compFamilyToCompArray.find(compFamily);
                if(foundCompArray != compFamilyToCompArray.end())
                {
                    // Add the component (which will deserialize the string), and set the owner ID
                    auto compArray = foundCompArray->second;
                    auto compIdx = compArray->add_item(compStr);
                    compArray->getBaseComponent(compIdx).ownerID = objectID;

                    // Update the component arrays and indices
                    obj.componentArrays[compFamily] = compArray;
                    obj.componentIndices[compFamily] = compIdx;
                    status = true;
                }
            }
        }
        else
            std::cerr << "Error: Component name not bound: " << compName << std::endl;
    }
    else
        std::cerr << "Error: Invalid object ID: " << objectID << std::endl;

    return status;
}

void ObjectManager::addComponentToPrototypeFromString(const std::string& prototypeName, const std::string& compName, const std::string& compValues)
{
    if(stringToCompFamily.find(compName) != stringToCompFamily.end())
    {
        auto componentFamily = stringToCompFamily[compName];

        //std::cout << "Adding component family: " << componentFamily << std::endl;

        //Get the prototype
        auto& prototype = objectPrototypes[prototypeName];

        if(prototype.objectID == ID(-1))
            prototype.objectID = prototypeIDCounter++;

        //If the prototype does not already have the given component
        if(prototype.componentArrays.find(componentFamily) == prototype.componentArrays.end())
        {
            //Add the first component in the list
            auto compArray = compFamilyToProtoCompArray[componentFamily];

            auto compIdx = compArray->add_item(compValues);

            prototype.componentArrays[componentFamily] = compArray;
            prototype.componentIndices[componentFamily] = compIdx;

        }

          //std::cout << "Finished Adding component family: " << componentFamily << std::endl;
    }
    else
        std::cerr << "Error: " << compName << " not bound to a component\n";
}

}//ocs
