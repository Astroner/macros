#if !defined(DEFER)

#define DEFER(start, end) for(start, *defer_INTERNAL = (void*)0x0; defer_INTERNAL != (void*)0x1; defer_INTERNAL = (end, (void*)0x1)) 

// Exit DEFER and run cleanup
#define DEFER_EXIT continue;

// EXIT DEFER with no cleanup
#define DEFER_BREAK break;

#define DEFER_SIMPLE(START, END)\
    for(int defer_INTERNAL = ((START), 0); defer_INTERNAL < 1; defer_INTERNAL += (END, 1))

// Defining resources
#define DEFER_RESOURCES(...)\
    struct {\
        int __flag;\
        __VA_ARGS__\
    } RAW_RES_DEFER

// Resources initialization
#define DEFER_I

// Initialized resources
#define DEFER_R RAW_RES_DEFER

// Resources initialization
#define DEFER_INIT(...)\
    {\
        .__flag = 1,\
        __VA_ARGS__\
    }

// Resources cleanup
#define DEFER_CLEANUP(...)\
    (__VA_ARGS__ 0)

// DEFER for many resources
#define DEFER_MANY(RESOURCES, INIT, CLEANUP)\
    for(RESOURCES = INIT; RAW_RES_DEFER.__flag != 0; RAW_RES_DEFER.__flag = CLEANUP)\

#endif // DEFER
#include <stdlib.h>
#include <stdio.h>

#if !defined(TODO)

#define INTERNAL_TODO_STR2(arg) #arg
#define INTERNAL_TODO_STR(arg) INTERNAL_TODO_STR2(arg)

/*
    TODO(text, args)
    TODO("add this feature")
    TODO("fix function for number %d", 4)
*/
#define TODO(...)\
    do {\
        printf("\x1B[1;33mReached TODO at "__FILE__":"INTERNAL_TODO_STR(__LINE__)"\n");\
        printf(__VA_ARGS__);\
        printf("\x1B[0m\n");\
        exit(1);\
    } while(0);\

#endif // TODO
