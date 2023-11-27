#if !defined(DEFER_H)
#define DEFER_H

#define DEFER(start, end) for(start, *i = 0x0; i != (void*)0x1; i = (end, (void*)0x1)) 
#define DEFER_EXIT continue;
#define DEFER_BREAK break;

#endif // DEFER_H
