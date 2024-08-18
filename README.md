# Hi there
It is a collection of macros I find useful

File [macros.h](https://raw.githubusercontent.com/Astroner/macros/master/macros.h) contains all the macros provided by this repo except [tests.h](https://raw.githubusercontent.com/Astroner/macros/master/tests.h)

| Name            | Link                                                                                | Description                                        |
|-----------------|-------------------------------------------------------------------------------------|----------------------------------------------------|
| [Defer](#defer-functions) | [defer.h](https://raw.githubusercontent.com/Astroner/macros/master/defer.h)         | Provides DEFER macro to manage temporal resources such as memory, files and e.t.c. |
| [TODO](#todo)   | [todo.h](https://raw.githubusercontent.com/Astroner/macros/master/todo.h)           | Provides TODO macro to make TODOs in code          |
|[Ancipe](#ancipe)| [ancipe.h](https://raw.githubusercontent.com/Astroner/macros/master/ancipe.h)       | Stdout coloring lib                                |
| [Tests](#tests) | [tests.h](https://raw.githubusercontent.com/Astroner/macros/master/tests.h) [tests-new.h](https://raw.githubusercontent.com/Astroner/macros/master/tests-new.h)         | Various macros for tests                           |

# Defer functions
 - [DEFER](#defer)
 - [DEFER_SIMPLE](#defer_simple)
 - [DEFER_MANY](#defer_many)

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

## DEFER
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

## DEFER_SIMPLE
**DEFER_SIMPLE** can be used if the resource does not require variable definitions(library initialization functions for example)
```c
#include <some-library.h>

#include "defer.h"

int main(void) {
    DEFER_SIMPLE(Library_INIT(), Library_QUIT()) {
        // ...code
    }
}
```

## DEFER_MANY
Use **DEFER_MANY** macro to manage more than one resource:
```c
#include <stdio.h>
#include "defer.h"

int main() {
    DEFER_MANY(
        DEFER_RESOURCES(
            FILE* first;
            FILE* second;
        ),
        DEFER_INIT(
            DEFER_I.first = fopen("example_1.txt"),
            DEFER_I.second = fopen("example_1.txt"),
        ),
        DEFER_CLEANUP(
            fclose(DEFER_R.first),
            fclose(DEFER_R.second),
        )
    ) {
        int f = getc(DEFER_R.first);
        int s = getc(DEFER_R.second);

        printf("%d, %d\n", f, s);
    }
}
```

**DEFER_MANY** accepts 3 other macros:
 - **DEFER_RESOURCES** - defines resources to use divided by **";"**.
 - **DEFER_INIT** - defines resource initialization. During initialization resources are accessed with **DEFER_I** macro. Each statement must be terminated by **","**.
 - **DEFER_CLEANUP** - defines cleanup. Each statement must be terminated by **","**. Initialized resources can be accessed with **DEFER_R** macro.

Then you can use initialized resources with **DEFER_R** macro.

**DEFER_BREAK** and **DEFER_EXIT** work the same as with **DEFER**

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

# Ancipe
Quick example:
```c
#include "ancipe.h"

int main(void) {
    acp(ACP_BLACK ACP_BG_WHITE, "Hello, world!");

    return 0;
}
```

**acp()** macro will print **"Hello, world!"** in black text(**ACP_BLACK**) on a white background(**ACP_BG_WHITE**).
**acp()** has the following declaration:
```c
#define acp(colors, format, ...)
```
 - **colors** - is stdout color configuration. Special **ACP_** prefixed constants divided by space.
 - **format** - format string
 - **...rest** - formatting arguments

Basically, **acp()** macro works the same with **printf()** except it requires coloring format as the first argument and adds a new line after.
> **acps()** macro does the same, but without a new line

**coloring format** consists of [text color](#text-color-list), [background color](#background-color-list) and [text style](#text-style-list). All of them are optional, but at least one parameter should be presented:
```c
acp(ACP_RED ACP_BOLD ACP_BG_WHITE, "Correct format");
acp(ACP_BOLD, "Also correct format");
acp("Incorrect format");
```

## Text color list
 - **ACP_BLACK**
 - **ACP_RED**
 - **ACP_GREEN**
 - **ACP_YELLOW**
 - **ACP_BLUE**
 - **ACP_MAGENTA**
 - **ACP_CYAN**
 - **ACP_WHITE**

## Background color list
 - **ACP_BG_RED**     
 - **ACP_BG_GREEN**   
 - **ACP_BG_YELLOW**  
 - **ACP_BG_BLUE**    
 - **ACP_BG_MAGENTA** 
 - **ACP_BG_CYAN**    
 - **ACP_BG_WHITE**   

## Text style list
 - **ACP_NORMAL**    
 - **ACP_BOLD**      
 - **ACP_UNDERLINE** 
 - **ACP_REVERSED**  

## Miscs
 - **acp_err()**, **acp_info()**, **acp_success()** cover general logging
 - Output can be disabled by defining **ACP_DISABLE** macro before **include** statement
 - Output dest can be overridden by defining **ACP_STD_PRINTF** macro before **include** statement
 - *ancipe* is pronounced as *an-see-pee*
     - The name comes from german word "anstreichen"
     - *an-s-treichen*
     - *an-C-treichen*
     - *treichen* can be translated into english as verb "to pet"
     - *an-C-pet*
     - *an-C-pe*
     - *ancipe*
# Tests 
Quick example:
```c
#include "tests-new.h"
#include "tests.h"

DESCRIBE(math) {
    IT("sums") {
        EXPECT(2 + 2) TO_BE(4)
    }

    IT("multiplies") {
        EXPECT(2 * 2) TO_BE(4)
    }
}
// Describing 'math'
//     it sums
//     it multiplies
```

## Content
 - [Test file structure](#test-file-structure)
 - [Matchers](#matchers)
     - [TO_BE](#to_be)
     - [TO_BE_TRUTHY](#to_be_truthy)
     - [TO_BE_FALSY](#to_be_falsy)
     - [TO_BE_STRING](#to_be_string)
     - [TO_BE_NULL](#to_be_null)
     - [TO_HAVE_BYTES](#to_have_bytes)
     - [TO_HAVE_RAW_BYTES](#to_have_raw_bytes)
 - [Test suit hooks](#test-suit-hooks)
     - [BEFORE_EACH](#before_each)
     - [AFTER_EACH](#after_each)
 - [Merging test suits](#merging-test-suits)
 - [Multiple test files](#multiple-test-files)
 - [Creating new matchers](#creating-new-matchers)
     - [CREATE_MATCHER_S](#create_matcher_s)
     - [CREATE_MATCHER](#create_matcher)
 - [Test notes](#test-notes)
 - [Printf like functions](#printf-like-functions)
     - [CREATE_PRINTF_LIKE_FUNCTION](#create_printf_like_function)
     - [CREATE_PRINTF_LIKE_FUNCTION_SIGNATURES](#create_printf_like_function_signatures)
     - [ADD_PRINTF_MOCK](#add_printf_mock)
 - [Overrides](#overrides)

## Test file structure
Each test file should begin with inclusion of [tests-new.h](https://raw.githubusercontent.com/Astroner/macros/master/tests-new.h) to do some macro preparations, only then [tests.h](https://raw.githubusercontent.com/Astroner/macros/master/tests.h) should be included.
Test starts with **DESCRIBE** macro, which initiates everything with provided label including main function. There can only be one **DESCRIBE** per program unless **MULTI_TEST**([More about multiple test files](#multiple-test-files)) or **WITH_DESCRIBE_MERGE**([more about merging test suits](#merging-test-suits)) are defined. **DESCRIBE** macro accepts test suit identifier/name.
**DESCRIBE** contains **IT** statements, actually describing test cases.
Then goes **EXPECT** with matchers. So the general structure looks like this:
```c
#include "tests-new.h"
#include "tests.h"

DESCRIBE(LABEL) {
    // ... it statements
    IT("LABEL") {
        EXPECT(something) // ... matcher
    }
}
```

Also you can add **IT_TODO** to create a test that will be implemented later.
```c
#include "tests-new.h"
#include "tests.h"

DESCRIBE(math) {
    IT_TODO("works")
}

// Describing 'math'
//     TODO: it works
```

## Matchers
Any matcher can be modified with **NOT**:
```c
#include "tests-new.h"
#include "tests.h"

DESCRIBE(math) {
    IT("sums") {
        EXPECT(2 + 2) NOT TO_BE(5);
    }
}
```

### TO_BE
Simplest matcher, test value to be equal to expected one
```c
#include "tests-new.h"
#include "tests.h"

DESCRIBE(math) {
    IT("sums") {
        EXPECT(2 + 2) TO_BE(4);
    }
}
```

### TO_BE_TRUTHY
Checks that value is reducible to **true**(1) via **!!** operation
```c
#include "tests-new.h"
#include "tests.h"

DESCRIBE(math) {
    IT("sums") {
        EXPECT(2 + 2) TO_BE_TRUTHY;
    }
}
```

### TO_BE_FALSY
[TO_BE_TRUTHY](#to_be_truthy) but in reverse
```c
#include "tests-new.h"
#include "tests.h"

DESCRIBE(math) {
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
#include "tests-new.h"
#include "tests.h"

DESCRIBE(strings) {
    IT("exists") {
        char* str = "test string";
        EXPECT(str) TO_BE_STRING("test string");
    }
}
```

### TO_BE_NULL
Assumes that provided value is a pointers and checks that it is **NULL**
```c
#include "tests-new.h"

#include <stdlib.h>

#include "tests.h"

DESCRIBE(mallocTests) {
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
#include "tests-new.h"
#include "tests.h"

DESCRIBE(charArrays) {
    IT("is comparable with string") {
        char array[] = { 'a', 'b' };

        EXPECT((char*)array) TO_HAVE_BYTES("ab", 2);
    }
}
```

### TO_HAVE_RAW_BYTES
Assumes that provided value is a pointer and compares it with provided bytes.
```c
#include "tests-new.h"
#include "tests.h"

DESCRIBE(charsArrays) {
    IT("it matches the buffer") {
        char* str = "ab";

        EXPECT(str) TO_HAVE_RAW_BYTES('a', 'b', '\0');
    }
}
```

## Test suit hooks
This section describes macros to add actions before/after each test.

### BEFORE_EACH
To use **BEFORE_EACH** statement you need to define **WITH_BEFORE_EACH** before including [tests.h](https://raw.githubusercontent.com/Astroner/macros/master/tests.h)
```c
#include "tests-new.h"

#define WITH_BEFORE_EACH
#include "tests.h"

int a = 0;
int b = 0;

BEFORE_EACH(math) {
    a = 2;
    b = 2;
}

DESCRIBE(math) {
    IT("sums") {
        EXPECT(a + b) TO_BE(4)
    }

    IT("multiplies") {
        EXPECT(a * b) TO_BE(4)
    }
}
```
**BEFORE_EACH** block will be executed before each **IT** statement. **BEFORE_EACH** statement should be initiated with the same **LABEL** that is passed to **DESCRIBE** statement action is attached to.

### AFTER_EACH
Works same as [BEFORE_EACH](#before_each) but code block is executed after each **IT** statement. To use it you also need to define **WITH_AFTER_EACH**
```c
#include "tests-new.h"

#define WITH_AFTER_EACH
#include "tests.h"

int result = 0;

AFTER_EACH(AFTER_EACH_SHOWCASE) {
    result = 2;
}

DESCRIBE(AFTER_EACH_SHOWCASE) {
    IT("does nothing here") {
        EXPECT(result) TO_BE(0)
    }

    IT("works here") {
        EXPECT(result) TO_BE(2)
    }
}
```

## Merging test suits
Quick example:
```c
#include "tests-new.h"

#define WITH_DESCRIBE_MERGE
#include "tests.h"

DESCRIBE(sub_module_1) {
    // tests for the sub module
}

DESCRIBE(sub_module_2) {
    // tests for the sub module
}

DESCRIBE_MERGE(module, sub_module_1, sub_module_2)

// Describing 'module'
//    Describing 'sub_module_1'
//        ...
//    
//    Describing 'sub_module_2'
//        ...
```

Essentially, **DESCRIBE_MERGE** does what the name says, it merges multiple describe statements into one.

Additionally, **DESCRIBE_MERGE** allows test file split if it is required due to tests size.
```c
// sub_module_1.spec.c
#include "tests-new.h"

#include "tests.h"

DESCRIBE(sub_module_1) {
    IT_TODO("works");
}


// sub_module_2.spec.c
#include "tests-new.h"

#include "tests.h"

DESCRIBE(sub_module_2) {
    IT_TODO("works");
}

// module.spec.c
#include "tests-new.h"

#define WITH_DESCRIBE_MERGE
#include "tests.h"

#include "sub_module_1.spec.c"
#include "sub_module_2.spec.c"

DESCRIBE_MERGE(module, sub_module_1, sub_module_2)


// Describing 'module'
//     Describing 'sub_module_1'
//         TODO: it works

//     Describing 'sub_module_2'
//         TODO: it works
```

## Multiple test files
Quick example:
```c
// test1.c
#include "tests-new.h"
#include "tests.h"

DESCRIBE(test1) {
    IT("works") {
        EXPECT(1) TO_BE_TRUTHY;
    }
}

// test2.c
#include "tests-new.h"
#include "tests.h"

DESCRIBE(test2) {
    IT("works") {
        EXPECT(1) TO_BE_TRUTHY;
    }
}

// main-test.c
#include "tests-new.h"

#define MULTI_TEST
#include "tests.h"

#include "test1.c"
#include "test2.c"

RUN_TESTS(test1, test2)

// stdout:
// Describing 'test1'
//     it works
//
// Describing 'test2'
//     it works
```
To run multiple test files at once, you need to define **MULTI_TEST** before including [tests.h](https://raw.githubusercontent.com/Astroner/macros/master/tests.h), then you can include different test files and at the end you can use macro **RUN_TESTS** with test identifiers to run tests.

## Creating new matchers
To create new matcher the library provides **CREATE_MATCHER** and **CREATE_MATCHER_S** macros.
Quick example
```c
#include "tests-new.h"
#include "tests.h"

#define TO_BE_2 CREATE_MATCHER_S(TO_BE_2, PASSES_IF(MATCHER_VALUE == 2), NO_EXPECTED)

DESCRIBE(custom_matcher) {
    IT("works") {
        EXPECT(3) TO_BE_2
    }
}
// stdout:
// Describing 'custom_matcher'
//     it works
//     Failed:
//         At main.c:8
//         EXPECT(3) TO_BE_2

```
### CREATE_MATCHER_S
Generally, **CREATE_MATCHER_S** is for simple matcher with a simple condition
```c
#define CREATE_MATCHER_S(MATCHER_NAME, CONDITION, EXPECTED_TEXT)
```
 - **MATCHER_NAME** - new matcher
 - **CONDITION** - matcher condition wrapped with **PASSES_IF** or **FAILS_IF**
 - **EXPECTED_TEXT** - expected value in a form of string wrapped with **EXPECTED** macro. If expected value is not necessary like in the example above, **NO_EXPECTED** macro can be passed

Value that was passed to **EXPECT** can be accessed with **MATCHER_VALUE** macro.

### CREATE_MATCHER
If matcher logic is more complicated and requires more workarounds with the value **CREATE_MATCHER** can be used.
```c
#include "tests-new.h"
#include "tests.h"

#define TO_BE_1000_WITH(VALUE)\
    CREATE_MATCHER(TO_BE_1000_WITH,\
        int value = MATCHER_VALUE + VALUE;\
        MATCHER_CONDITION_S(PASSES_IF(value == 1000), EXPECTED("1000 - "#VALUE));\
    )

DESCRIBE(custom_matcher) {
    IT("works") {
        EXPECT(20) TO_BE_1000_WITH(1);
    }
}

// stdout:
// Describing 'custom_matcher'
//     it works
//     Failed:
//         At main.c:12
//         EXPECT(20) TO_BE_1000_WITH(1000 - 1)
```
Unlike **CREATE_MATCHER_S** **CREATE_MATCHER** can work with provided code
```c
#define CREATE_MATCHER(MATCHER_NAME, ...)
```
 - **MATCHER_NAME** - new matcher
 - **VARIADIC ARGUMENTS** - matcher body

In the example we use **MATCHER_CONDITION_S** macro to fulfill matcher condition
```c
#define MATCHER_CONDITION_S(CONDITION, EXPECTED_TEXT)
```
 - **CONDITION** - matcher condition wrapped with **PASSES_IF** or **FAILS_IF**
 - **EXPECTED_TEXT** - expected value in a form of string wrapped with **EXPECTED** macro or **NO_EXPECTED**;

But if even more workaround is required **MATCHER_CONDITION** macro can be used instead:
```c
#include "tests-new.h"
#include "tests.h"

#include "stdlib.h"

#define TO_BE_1000_WITH(VALUE)\
    CREATE_MATCHER(TO_BE_1000_WITH,\
        int value = MATCHER_VALUE + VALUE;\
        void* mem = malloc(1000);\
        int passed;\
        MATCHER_CONDITION(PASSES_IF(value == 1000)) {\
            passed = 0;\
            MATCHER_INFO("= Test failed =");\
        } else {\
            passed = 1;\
        }\
        free(mem);\
        if(!passed) {\
            MATCHER_FAIL(EXPECTED("1000 - "#VALUE));\
        }\
    )

DESCRIBE(custom_matcher) {
    IT("works") {
        EXPECT(20) TO_BE_1000_WITH(1);
    }
}
// stdout:
// Describing 'custom_matcher'
//     it works
//     [INFO] = Test failed =
//     Failed:
//         At main.c:25
//         EXPECT(20) TO_BE_1000_WITH    (1000 - 1)
```
In this example we can allocate some memory during matching process and free it at the end.
**MATCHER_CONDITION** works as a simple **if** statement and the first code block will be executed if the condition fails.
```c
#define MATCHER_CONDITION(CONDITION)
```
 - **CONDITION** - matcher condition wrapped with **PASSES_IF** or **FAILS_IF**

**MATCHER_FAIL** macro fails the matcher
```c
#define MATCHER_FAIL(EXPECTED_TEXT)
```
 - **EXPECTED_TEXT** - expected value in a form of string wrapped with **EXPECTED** macro or **NO_EXPECTED**.

To output some additional info you can use macro **MATCHER_INFO**, which has the same format as the **printf** function, but with an automatic newline at the end.

## Test notes
 - Basically, if you have only one test file or you do not plan to use **MULTI_TEST** related features, it is not required to include [tests-new.h](https://raw.githubusercontent.com/Astroner/macros/master/tests-new.h)
 - You do not need to include [tests-new.h](https://raw.githubusercontent.com/Astroner/macros/master/tests-new.h) if you are going to use only [CREATE_PRINTF_LIKE_FUNCTION](#create_printf_like_function) collection.
 - Test macros are designed to be executed one by one or with **MULTI_TEST** feature enabled.
 - You can change tests output tab width by defining **TESTS_PRINT_TAB_WIDTH** macro with specified number of spaces(by default it is 4) before including [tests.h](https://raw.githubusercontent.com/Astroner/macros/master/tests.h)

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
 - **__nextString()** - returns pointer to the next written content. Example:
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

### CREATE_PRINTF_LIKE_FUNCTION_SIGNATURES
Defines only signatures from [CREATE_PRINTF_LIKE_FUNCTION](#create_printf_like_function)
```c
// common.h
#include "tests.h"

CREATE_PRINTF_LIKE_FUNCTION_SIGNATURES(testPrint)
void doSomething(void);


// some-file.c
#include "common.h"

void doSomething(void) {
    testPrint("text\n");
}


// main.c
#include "common.h";

CREATE_PRINTF_LIKE_FUNCTION(testPrint, 200)

int main(void) {
    doSomething();
    testPrint("another text\n");

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