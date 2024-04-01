#if !defined(DEFER)

#define DEFER(start, end) for(start, *i = (void*)0x0; i != (void*)0x1; i = (end, (void*)0x1)) 

// Exit DEFER and run cleanup
#define DEFER_EXIT continue;

// EXIT DEFER with no cleanup
#define DEFER_BREAK break;

// Defining resources
#define DEFER_RESOURCES(RESOURCE, ...)\
    struct {\
        int __flag;\
        RESOURCE;\
        __VA_ARGS__\
    } RAW_RES_DEFER

// Resources initialization
#define DEFER_I

// Initialized resources
#define DEFER_R RAW_RES_DEFER

// Resources initialization
#define DEFER_INIT(STATEMENT, ...)\
    {\
        .__flag = 1,\
        STATEMENT,\
        __VA_ARGS__\
    }

// Resources cleanup
#define DEFER_CLEANUP(STATEMENT, ...)\
    (STATEMENT, __VA_ARGS__ 0)

// DEFER for many resources
#define DEFER_MANY(RESOURCES, INIT, CLEANUP)\
    for(RESOURCES = INIT; RAW_RES_DEFER.__flag != 0; RAW_RES_DEFER.__flag = CLEANUP)\

#endif // DEFER
