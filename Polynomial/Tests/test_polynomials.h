#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <assert.h>
#include "criterion/criterion.h"
#include "polynomial.h"

#define TEST_INPUT_DIR "tests.in"
#define TEST_OUTPUT_DIR "tests.out"
#define TEST_TIMEOUT 30

int run_using_system(char *test_name);
void expect_normal_exit(int status);
void expect_error_exit(int actual_status, int expected_status);
void expect_no_valgrind_errors(int status);
void report_return_value(int return_act, int return_exp);
bool arePolynomialsEqual(const Polynomial *polynomial_1, const Polynomial *polynomial_2);