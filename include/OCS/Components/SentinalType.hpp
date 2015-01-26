#ifndef OCS_SENTINAL_TYPE_H
#define OCS_SENTINAL_TYPE_H

#include <OCS/Components/Component.hpp>
namespace ocs
{
    //!Used to end the recursive calls that use variadic templates.
    struct SentinalType : public ocs::Component<SentinalType> 
    {
        template<typename T>
        static bool endRecursion(const T&) { return false; }
        static bool endRecursion(const SentinalType&) { return true; };        
    };
}

#endif