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
