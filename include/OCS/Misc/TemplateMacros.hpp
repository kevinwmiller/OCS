#ifndef TEMPLATE_MACROS_H
#define TEMPLATE_MACROS_H

#define CREATE_HAS_MEMBER_FUNCTION(func, functionName) \
    template<typename T> \
    struct functionName { \
        typedef char yes[1]; \
        typedef char no[2]; \
\
        template <typename C> \
        static yes& test( decltype(&C::func)); \
\
        template <typename C> \
        static no& test(...); \
\
        static bool const value = sizeof(test<T>(0)) == sizeof(yes); \
    };

#endif