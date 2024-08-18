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
#ifndef ANCIPE_H
#define ANCIPE_H


#if !defined(ACP_STD_PRINTF)
    #include <stdio.h>
    #define ACP_STD_PRINTF printf
#endif // ACP_STD_PRINTF


#define ACP_RESET   "\x1b[0m"

// Text color codes
#define ACP_BLACK   "\x1b[30m"
#define ACP_RED     "\x1b[31m"
#define ACP_GREEN   "\x1b[32m"
#define ACP_YELLOW  "\x1b[33m"
#define ACP_BLUE    "\x1b[34m"
#define ACP_MAGENTA "\x1b[35m"
#define ACP_CYAN    "\x1b[36m"
#define ACP_WHITE   "\x1b[37m"

// Text style codes
#define ACP_NORMAL    "\x1b[22m"
#define ACP_BOLD      "\x1b[1m"
#define ACP_UNDERLINE "\x1b[4m"
#define ACP_REVERSED  "\x1b[7m"

// Background color codes
#define ACP_BG_RED     "\x1b[41m"
#define ACP_BG_GREEN   "\x1b[42m"
#define ACP_BG_YELLOW  "\x1b[43m"
#define ACP_BG_BLUE    "\x1b[44m"
#define ACP_BG_MAGENTA "\x1b[45m"
#define ACP_BG_CYAN    "\x1b[46m"
#define ACP_BG_WHITE   "\x1b[47m"


#ifdef ACP_DISABLE
    #define acp(colors, ...)
    #define acps(colors, ...)
#else
    #define acp(colors, ...)\
        do {\
            ACP_STD_PRINTF(colors);\
            ACP_STD_PRINTF(__VA_ARGS__);\
            ACP_STD_PRINTF(ACP_RESET "\n");\
        } while(0)

    #define acps(colors, ...)\
        do {\
            ACP_STD_PRINTF(colors);\
            ACP_STD_PRINTF(__VA_ARGS__);\
            ACP_STD_PRINTF(ACP_RESET);\
        } while(0)
#endif

#define acp_err(...) acp(ACP_RED ACP_BOLD, __VA_ARGS__)
#define acp_info(...) acp(ACP_YELLOW ACP_BOLD, __VA_ARGS__)
#define acp_success(...) acp(ACP_GREEN ACP_BOLD, __VA_ARGS__)

#endif // ANCIPE_H
