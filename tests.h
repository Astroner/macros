#if !defined(TESTS_H)
#define TESTS_H

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define CREATE_PRINTF_LIKE_FUNCTION(NAME, BUFFER_SIZE)\
    char NAME##__buffer[BUFFER_SIZE + 1] = { '\0' };\
    char NAME##__length = 0;\
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
        NAME##__buffer[BUFFER_SIZE] = '\0';\
        size_t writtenLength = strlen(NAME##__buffer + NAME##__length);\
        NAME##__length += writtenLength;\
\
        return writtenLength;\
    }\
    \
    void NAME##__reset() {\
        NAME##__length = 0;\
    }\

#define ADD_PRINTF_MOCK(BUFFER_SIZE) CREATE_PRINTF_LIKE_FUNCTION(printf, BUFFER_SIZE);

#define DESCRIBE(LABEL) \
    int runTests(int* TESTS_failLine, char** TESTS_testLabel,char** TESTS_testText, char** TESTS_operatorText, char** TESTS_expectText);\
    int main(void) {\
        printf("\n\nDescribing '"LABEL"'\n");\
        int failLine;\
        char* testLabel;\
        char* testText;\
        char* operatorText;\
        char* expectText;\
        if(runTests(&failLine, &testLabel, &testText, &operatorText, &expectText) < 0) {\
            printf("it %s ... FAILED\n", testLabel);\
            printf("    At %s:%d\n", __FILE__, failLine);\
            if(expectText != NULL) {\
                printf("    EXPECT(%s) %s(%s)\n", testText, operatorText, expectText);\
            } else {\
                printf("    EXPECT(%s) %s\n", testText, operatorText);\
            }\
        }\
\
        printf("\n\n");\
        return 0;\
    }\
    int runTests(int* TESTS_failLine, char** TESTS_testLabel,char** TESTS_testText, char** TESTS_operatorText, char** TESTS_expectText)

#define IT(LABEL)\
    for(char i = 0, *testCaseLabel = *TESTS_testLabel = LABEL; i < 1; i += (printf("it "LABEL" ... PASSED\n"), 1))

#define EXPECT(VALUE)\
    if(1) {\
        __typeof__((VALUE)) localPassedValue = (VALUE);\
        *TESTS_failLine = __LINE__;\
        *TESTS_testText = #VALUE;\
        int isNot = 0;

#define NOT\
    isNot = 1;

#define TO_BE(EXPECTED)\
        if((localPassedValue != (EXPECTED)) ^ isNot) {\
            *TESTS_expectText = #EXPECTED;\
            *TESTS_operatorText = "TO_BE";\
            return -1;\
        }\
    };\

#define TO_BE_TRUTHY\
    if(!localPassedValue) {\
        *TESTS_expectText = NULL;\
        *TESTS_operatorText = "TO_BE_TRUTHY";\
        return -1;\
    }\
};\

#define TO_BE_FALSY\
    if(!localPassedValue) {\
        *TESTS_expectText = NULL;\
        *TESTS_operatorText = "TO_BE_FALSY";\
        return -1;\
    }\
};\

#endif // TESTS_H
