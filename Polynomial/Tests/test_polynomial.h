#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <assert.h>
#include "../criterion/criterion.h"
#include "../../include/polynomial.h"

#define TEST_INPUT_DIR "tests.in"
#define TEST_OUTPUT_DIR "tests.out"
#define TEST_TIMEOUT 30 

int run_using_system(char *test_name);
void assert_normal_exit(int status);
void assert_error_exit(int status);
void assert_no_valgrind_errors(int status);
