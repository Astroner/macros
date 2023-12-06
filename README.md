# Hi there
It is a collection of macros I find useful

File [macros.h](https://raw.githubusercontent.com/Astroner/macros/master/macros.h) contains all the macros provided by this repo

| Name | Link | Description |
| --- | --- | --- |
| [Defer](#defer) | [defer.h](https://raw.githubusercontent.com/Astroner/macros/master/defer.h) | Provides DEFER macro to manage temporal resources such as memory, files and e.t.c. |
| [TODO](#todo) | [todo.h](https://raw.githubusercontent.com/Astroner/macros/master/todo.h) | Provides TODO macro to make TODOs in code |
| [Tests](#tests) | [tests.h](https://raw.githubusercontent.com/Astroner/macros/master/tests.h) | Various macros for tests |

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

Also this macro accepts **printf** like syntax:
```c
#include "todo.h"

int main(void) {
    TODO("Todo at line %d", __LINE__);

    return 0;
}
```

# Tests 
Quick example:
```c
#include "tests.h"

DESCRIBE("math") {
    IT("sums") {
        EXPECT(2 + 2) TO_BE(4)
    }

    IT("multiplies") {
        EXPECT(2 * 2) TO_BE(4)
    }
}
// Describing 'math'
// it sums
// it multiplies
```

## Content
 - [Structure](#structure)
 - [Matchers](#matchers)
     - [TO_BE](#to_be)
     - [TO_BE_TRUTHY](#to_be_truthy)
     - [TO_BE_FALSY](#to_be_falsy)
     - [TO_BE_STRING](#to_be_string)
     - [TO_BE_NULL](#to_be_null)
     - [TO_HAVE_BYTES](#to_have_bytes)
 - [Test suit hooks](#test-suit-hooks)
     - [BEFORE_EACH](#before_each)
     - [AFTER_EACH](#after_each)
 - [Printf like functions](#printf-like-functions)
     - [CREATE_PRINTF_LIKE_FUNCTION](#create_printf_like_function)
     - [ADD_PRINTF_MOCK](#add_printf_mock)
 - [Overrides](#overrides)

## Structure
Test starts with **DESCRIBE** macro, which initiates everything with provided label. There can only be one **DESCRIBE** per program.
Inside of **DESCRIBE** **IT** constructions go, actually describing test cases.
Then go **EXPECT** with matchers. So the general structure looks like this:
```c
#include "tests.h"

DESCRIBE("LABEL") {
    // ... it statements
    IT("LABEL") {
        EXPECT(something) // ... matcher
    }
}
```

## Matchers
Any matcher can be modified with **NOT**:
```c
#include "tests.h"

DESCRIBE("math") {
    IT("sums") {
        EXPECT(2 + 2) NOT TO_BE(5);
    }
}
```

### TO_BE
Simplest matcher, test value to be equal to expected one
```c
#include "tests.h"

DESCRIBE("math") {
    IT("sums") {
        EXPECT(2 + 2) TO_BE(4);
    }
}
```

### TO_BE_TRUTHY
Checks that value is reducible to **true**(1) via **!!** operation
```c
#include "tests.h"

DESCRIBE("math") {
    IT("sums") {
        EXPECT(2 + 2) TO_BE_TRUTHY;
    }
}
```

### TO_BE_FALSY
[TO_BE_TRUTHY](#to_be_truthy) but in reverse
```c
#include "tests.h"

DESCRIBE("math") {
    IT("sums") {
        EXPECT(2 - 2) TO_BE_FALSY;
    }
}
```

### TO_BE_STRING
Assumes that provided value is pointer to a string and with function **TESTS_STD_STRCMP** compares provided and expected strings.
> **TESTS_STD_STRCMP** function can be overridden by defining it before including [tests.h](https://raw.githubusercontent.com/Astroner/macros/master/tests.h).

> By default it uses string's **strcmp**

```c
#include "tests.h"

DESCRIBE("strings") {
    IT("exists") {
        char* str = "test string";
        EXPECT(str) TO_BE_STRING("test string");
    }
}
```

### TO_BE_NULL
Assumes that provided value is a pointers and checks that it is **NULL**
```c
#include <stdlib.h>

#include "tests.h"

DESCRIBE("malloc") {
    IT("it allocates") {
        char* mem = malloc(22);
        EXPECT(mem) NOT TO_BE_NULL;
    }
}
```

### TO_HAVE_BYTES
Assumes that provided value is a pointer and compares it with expected bytes with function **TESTS_STD_MEMCMP**.
> **TESTS_STD_MEMCMP** function can be overridden by defining it before including [tests.h](https://raw.githubusercontent.com/Astroner/macros/master/tests.h)

> By default it uses string's **memcmp**
```c
#include "tests.h"

DESCRIBE("char arrays") {
    IT("is comparable with string") {
        char array[] = { 'a', 'b' };

        EXPECT((char*)array) TO_HAVE_BYTES("ab", 2);
    }
}
```

## Test suit hooks
This section describes macros to add actions before/after each test.

### BEFORE_EACH
To use **BEFORE_EACH** statement you need to define **WITH_BEFORE_EACH** before including [tests.h](https://raw.githubusercontent.com/Astroner/macros/master/tests.h)
```c
#define WITH_BEFORE_EACH
#include "tests.h"

int a = 0;
int b = 0;

BEFORE_EACH {
    a = 2;
    b = 2;
}

DESCRIBE("math") {
    IT("sums") {
        EXPECT(a + b) TO_BE(4)
    }

    IT("multiplies") {
        EXPECT(a * b) TO_BE(4)
    }
}
```
**BEFORE_EACH** block will be executed before each **IT** statement

### AFTER_EACH
Works same as [BEFORE_EACH](#before_each) but code block is executed after each **IT** statement. To use it you also need to define **WITH_AFTER_EACH**
```c
#define WITH_AFTER_EACH
#include "tests.h"

int result = 0;

AFTER_EACH {
    result = 2;
}

DESCRIBE("AFTER_EACH") {
    IT("does nothing here") {
        EXPECT(result) TO_BE(0)
    }

    IT("works here") {
        EXPECT(result) TO_BE(2)
    }
}
```

## Printf like functions
[tests.h](https://raw.githubusercontent.com/Astroner/macros/master/tests.h) provides several macros to define **printf** like functions

### CREATE_PRINTF_LIKE_FUNCTION
**CREATE_PRINTF_LIKE_FUNCTION(NAME, BUFFER_LENGTH)** defines function with provided **NAME** that has signature **int NAME(const char* format, ...)** and imitates **printf** function/macro but writes into buffer with length **BUFFER_LENGTH**. Macro defines several variables and functions prefixed with **NAME**. 
For example, if **NAME** equals to **"testPrint"** then the buffer can be accessed with **testPrint__buffer**.

List if defined variables/functions:
 - **__buffer** - buffer where created function writes
 - **__length** - length of the currently written content
 - **__lastString** - pointer to the last written string
 - **__lastStringLength** - length of the last written length
 - **__reset()** - resets the buffer
 - **__nextString** - returns pointer to the next written content. Example:
    ```c
    #include "tests.h"
    CREATE_PRINTF_LIKE_FUNCTION(testPrint, 200);

    int main(void) {
        char* next = testPrint__nextString();

        testPrint("%d", 22);

        printf("written string: %s\n", next); // written string: 22

        return 0;
    }
    ```

### ADD_PRINTF_MOCK
**ADD_PRINTF_MOCK** macro creates function **printf** identical to stdio's signature that writes into created buffer. 
This macro uses [CREATE_PRINTF_LIKE_FUNCTION](#create_printf_like_function) under the hood so all the useful variables and functions are provided
```c
#include <stdio.h>
#include "tests.h"

ADD_PRINTF_MOCK(200)

int main(void) {
    printf("number: %d", 44);

    fprintf(stdout, "Last string: '%s'\n", printf__lastString); // Last string: 'number: 44'

    return 0;
}
```

## Overrides
[tests.h](https://raw.githubusercontent.com/Astroner/macros/master/tests.h) can be used as a almost completely standalone header file that depends only on *stdarg.h* and *stddef.h* by defining all required functions before including [tests.h](https://raw.githubusercontent.com/Astroner/macros/master/tests.h)

List of overrides:
| Macro name       | Default value         | Usage                                                                                                        |
|------------------|-----------------------|--------------------------------------------------------------------------------------------------------------|
| TESTS_STD_PRINT  | stdio's **printf()**  | Used for logging test related info into stdout                                                               |
| TESTS_STD_STRLEN | string's **strlen()** | Used for measuring logged string length in [CREATE_PRINTF_LIKE_FUNCTION](#create_printf_like_function) macro |
| TESTS_STD_STRCMP | string's **strcmp()** | Used in **TO_BE_STRING** matcher to check strings                                                            |
| TESTS_STD_MEMCMP | string's **memcmp()** | Used in **TO_HAVE_BYTES** matcher to check memory                                                            |