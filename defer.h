#if !defined(DEFER)

#define DEFER(start, end) for(start, *i = 0x0; i != (void*)0x1; i = (end, (void*)0x1)) 

// Exit DEFER and run cleanup
#define DEFER_EXIT continue;

// EXIT DEFER with no cleanup
#define DEFER_BREAK break;

#endif // DEFER
