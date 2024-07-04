#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

typedef int (* test_func_t)(void*arg);

typedef struct
{
    const char* component_type;
    unsigned int test_case_number;
    char * test_description;
    void* arg;
    test_func_t init;
    test_func_t run;
    test_func_t exit;
}test_case_t;

int run_tests(const char* component_type, unsigned int test_number);

#endif // TEST_FRAMEWORK_H

