#if !defined(DEFER)
#define DEFER

#define DEFER(start, end) for(start, *i = 0x0; i != (void*)0x1; i = (end, (void*)0x1)) 
#define DEFER_EXIT continue;
#define DEFER_BREAK break;

#endif // DEFER
#include <stdlib.h>
#include <stdio.h>

#if !defined(TODO)

#define TODO(text)\
    do {\
        printf("Reached TODO at %s:%d\n%s\n", __FILE__, __LINE__, text);\
        exit(1);\
    } while(0);\

#endif // TODO
