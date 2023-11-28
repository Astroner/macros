# Hi there
It is a collection of macros I find useful

| Name | Link | Description |
| --- | --- | --- |
| [Defer](#defer) | [defer.h](https://raw.githubusercontent.com/Astroner/macros/master/defer.h) | Provides DEFER macro to manage temporal resources such as memory, files and e.t.c. |
| [TODO](#todo) | [todo.h](https://raw.githubusercontent.com/Astroner/macros/master/todo.h) | Provides TODO macro to make TODOs in code |

# Defer
Quick example:
```c
#include <stdio.h>
#include "defer.h"

int main(int argc, char** argv) {
    if(argc < 2) {
        printf("Not input file\n");
        return 1;
    }

    char* filePath = argv[1];

    DEFER(FILE* f = fopen(filePath, "r"), fclose(f)) {
        char ch;
        while((ch = getc(f)) != EOF) {
            putchar(ch);
        }
        putchar('\n');
    }

    return 0;
}
```

**DEFER** macro here helps to manage opened file. When the code block ends, **fclose(f)** will be called automatically.
In cases when you want to *exit* **DEFER** statement by yourself, in case of error for example, use **DEFER_EXIT** macro:
```c
#include <stdio.h>
#include "defer.h"

int main(int argc, char** argv) {
    if(argc < 2) {
        printf("Not input file\n");
        return 1;
    }

    char* filePath = argv[1];

    DEFER(FILE* f = fopen(filePath, "r"), fclose(f)) {
        char ch = getc(f);
        if(ch == 'a') {
            DEFER_EXIT;
        }
        while((ch = getc(f)) != EOF) {
            putchar(ch);
        }
        putchar('\n');
    }

    return 0;
}
```
In this scenario **fclose(f)** will still be called.

If you want to *break* **DEFER**, use **DEFER_BREAK**.
```c
#include <stdio.h>
#include "defer.h"

int main(int argc, char** argv) {
    if(argc < 2) {
        printf("Not input file\n");
        return 1;
    }

    char* filePath = argv[1];

    DEFER(FILE* f = fopen(filePath, "r"), fclose(f)) {
        if(!f) DEFER_BREAK;

        char ch;
        while((ch = getc(f)) != EOF) {
            putchar(ch);
        }
        putchar('\n');
    }

    return 0;
}
```
**DEFER_BREAK** does not trigger cleanup, so you have to do it by yourself in certain cases.

> Note, that DEFER works via **for** statement, so it is impossible to **DEFER_EXIT** or **DEFER_BEAK** from nested loops or switch

# TODO
Quick example:
```c
#include "todo.h"

int main(void) {
    TODO("text");

    return 0;
}
```
When this program will reach **TODO** statement it will log into stdout message
> Reached TODO at main.c:4
> text

and then it will exit with code 1