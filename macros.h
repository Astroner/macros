#if !defined(DEFER)
#define DEFER

#define DEFER(start, end) for(start, *i = 0x0; i != (void*)0x1; i = (end, (void*)0x1)) 
#define DEFER_EXIT continue;
#define DEFER_BREAK break;

#endif // DEFER
#include <stdlib.h>
#include <stdio.h>

#if !defined(TODO)

#define TODO_STR2(arg) #arg
#define TODO_STR(arg) TODO_STR2(arg)

/*
    TODO(text, args)
    TODO("add this feature")
    TODO("fix function for number %d", 4)
*/
#define TODO(...)\
    do {\
        printf("Reached TODO at "__FILE__":"TODO_STR(__LINE__)"\n");\
        printf(__VA_ARGS__);\
        putchar('\n');\
        exit(1);\
    } while(0);\

#endif // TODO
#if !defined(TESTS_H)
#define TESTS_H

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define CREATE_PRINTF_LIKE_FUNCTION(NAME, BUFFER_SIZE)\
    char NAME##__buffer[BUFFER_SIZE + 1] = { '\0' };\
    char NAME##__length = 0;\
    char* NAME##__lastString = NAME##__buffer;\
    size_t NAME##__lastStringLength = 0;\
\
    int NAME(const char* format, ...) {\
        if(NAME##__length == BUFFER_SIZE) {\
            return -1;\
        }\
        va_list argptr;\
        va_start(argptr, format);\
        vsnprintf(NAME##__buffer + NAME##__length, BUFFER_SIZE - NAME##__length + 1, format, argptr);\
        va_end(argptr);\
\
        NAME##__lastString = NAME##__buffer + NAME##__length;\
\
        NAME##__buffer[BUFFER_SIZE] = '\0';\
        NAME##__lastStringLength = strlen(NAME##__buffer + NAME##__length);\
        NAME##__length += NAME##__lastStringLength;\
\
        return NAME##__lastStringLength;\
    }\
    \
    void NAME##__reset() {\
        NAME##__length = 0;\
    }\

#define ADD_PRINTF_MOCK(BUFFER_SIZE) CREATE_PRINTF_LIKE_FUNCTION(printf, BUFFER_SIZE);

#define DESCRIBE(LABEL) \
    int runTests(\
        int* TESTS_failLine, \
        char** TESTS_testLabel,\
        char** TESTS_testText, \
        int* TESTS_isNotFlag,\
        char** TESTS_operatorText,\
        char** TESTS_expectText\
    );\
    int main(void) {\
        printf("\n\nDescribing '"LABEL"'\n");\
        int failLine;\
        char* testLabel;\
        char* testText;\
        int isNot;\
        char* operatorText;\
        char* expectText;\
        if(runTests(&failLine, &testLabel, &testText, &isNot, &operatorText, &expectText) < 0) {\
            printf("\nFailed test:\nit %s\n", testLabel);\
            printf("    At %s:%d\n", __FILE__, failLine);\
            printf("    EXPECT(%s)%s%s", testText, isNot ? " NOT " : " ", operatorText);\
            if(expectText != NULL) {\
                printf("(%s)", expectText);\
            }\
            printf("\n");\
        }\
\
        printf("\n\n");\
        return 0;\
    }\
    int runTests(\
        int* TESTS_failLine, \
        char** TESTS_testLabel,\
        char** TESTS_testText, \
        int* TESTS_isNotFlag,\
        char** TESTS_operatorText,\
        char** TESTS_expectText\
    )

#define IT(LABEL)\
    for(char i = 0, *testCaseLabel = *TESTS_testLabel = LABEL; i < 1; i += (printf("it "LABEL"\n"), 1))

#define EXPECT(VALUE)\
    if(1) {\
        __typeof__((VALUE)) localPassedValue = VALUE;\
        *TESTS_failLine = __LINE__;\
        *TESTS_testText = #VALUE;\
        *TESTS_isNotFlag = 0;\

#define NOT\
    *TESTS_isNotFlag = 1;\

#define TO_BE(EXPECTED)\
        if((localPassedValue != (EXPECTED)) ^ *TESTS_isNotFlag) {\
            *TESTS_expectText = #EXPECTED;\
            *TESTS_operatorText = "TO_BE";\
            return -1;\
        }\
    };\

#define TO_BE_TRUTHY\
    if((!localPassedValue) ^ *TESTS_isNotFlag) {\
        *TESTS_expectText = NULL;\
        *TESTS_operatorText = "TO_BE_TRUTHY";\
        return -1;\
    }\
};\

#define TO_BE_FALSY\
    if((!localPassedValue) ^ *TESTS_isNotFlag) {\
        *TESTS_expectText = NULL;\
        *TESTS_operatorText = "TO_BE_FALSY";\
        return -1;\
    }\
};\

#define TO_BE_STRING(EXPECTED_STRING)\
    if((strcmp(localPassedValue, EXPECTED_STRING) != 0) ^ *TESTS_isNotFlag) {\
        *TESTS_expectText = #EXPECTED_STRING;\
        *TESTS_operatorText = "TO_BE_STRING";\
        return -1;\
    }\
};\

#endif // TESTS_H
