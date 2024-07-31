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
