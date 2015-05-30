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

#ifndef OCS_COMPONENT_H
#define OCS_COMPONENT_H

#include <type_traits>

#include "OCS/Misc/Config.hpp"
#include "OCS/Utilities/Serializer.hpp"
#include "OCS/Misc/TemplateMacros.hpp"

namespace ocs
{

class ObjectManager;

/*! \brief Components should not inherit from this struct
*          Please see Component<Derived>
*
*          BaseComponent is used internally by the Component template struct to keep track of
*          registered component IDs
*
*/
struct BaseComponent
{
    virtual ~BaseComponent() {}

    ID getOwnerID() const { return ownerID; }

    virtual std::string serialize() { return (""); }
    virtual void deSerialize(const std::string&) {}
    virtual const std::string& getName() = 0;

    protected:

        Serializer serializer;

        friend class ObjectManager;

        ID ownerID;
        static Family familyCounter;
        static std::string emptyName;
};

CREATE_HAS_MEMBER_FUNCTION(_name_, hasName);

/*! \brief All created components should inherit from this in the following way:
 *
 *         struct Position : public Component<Position>
 *
 * Derived components may implement serialize and deSerialize functions if the user wishes to load them
 * from a file or pass the component's information in a string.
 *
 * All derived components MUST implement a default constructor and SHOULD implement a paramaterized constructor.
 *
 */
template<typename Derived>
struct Component : public BaseComponent
{
    virtual ~Component() {}


    /*! \brief Assigns a family id if the component does not already have one.
    *          Returns the family id for the given component
    *
    *   \return The family id of the component.
    */
    static Family componentFamily()
    {
        static Family _family = BaseComponent::familyCounter++;
        return _family;
    }

    const std::string& getName()
    {
        return componentName();
    }

    static const std::string& componentName()
    {
        std::cout << "In component name. Has name?: " << hasName<Derived>::value << "\n";
        return ComponentName<Derived, hasName<Derived>::value>::name();
    }

protected:

    template <typename T, bool>
    struct ComponentName
    {
        static const std::string& name() { return emptyName; }
    };

    template <typename T>
    struct ComponentName<T, true>
    {
        static const std::string& name() { return T::_name_(); }
    };

};

}//ocs

#endif
