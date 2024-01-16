#include "test_polynomial.h"
#include "../include/polynomial.h"

static char test_log_outfile[100];

int run_using_system(char *test_name) {
    char executable[100];
    sprintf(executable, "./bin/%s", test_name);
    assert(access(executable, F_OK) == 0);

    char cmd[500];
    sprintf(test_log_outfile, "%s/%s.log", TEST_OUTPUT_DIR, test_name);
#if defined(__linux__)
    sprintf(cmd, "valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes --error-exitcode=37 ./bin/%s > %s 2>&1",
	    test_name, test_log_outfile);
#else
    cr_log_warn("Skipping valgrind tests. Run tests on Linux or GitHub for full output.\n");
    sprintf(cmd, "./bin/%s > %s 2>&1", test_name, test_log_outfile);
#endif
    return system(cmd);
}

void assert_normal_exit(int status) {
    cr_expect_eq(status, 0, "The program did not exit normally (status = 0x%x).\n", status);
}

void assert_error_exit(int status) {
    cr_expect_eq(WEXITSTATUS(status), 0xff,
		 "The program did not exit with status 0xff (status was 0x%x).\n", status);
}

void assert_no_valgrind_errors(int status) {
    cr_expect_neq(WEXITSTATUS(status), 37, "Valgrind reported errors -- see %s", test_log_outfile);
    if (WEXITSTATUS(status) == 37) {
        char cmd[200];
        sprintf(cmd, "cat %s", test_log_outfile);
        system(cmd);
    }
}

TestSuite(base_suite, .timeout=TEST_TIMEOUT);

Test(polynomial_tests, add_polynomials) {
    Polynomial addend_1, addend_2, sum;

    initializePolynomial(&addend_1, 2);
    setCoefficient(&addend_1, 2, 2.0);
    setCoefficient(&addend_1, 1, -1.0);
    setCoefficient(&addend_1, 0, 0.3);

    initializePolynomial(&addend_2, 2);
    setCoefficient(&addend_2, 2, 2.0);
    setCoefficient(&addend_2, 1, 4.3);
    setCoefficient(&addend_2, 0, 0.69);

    sum = addPolynomials(&addend_1, &addend_2);

    cr_assert_eq(sum.degree, 2);
    cr_assert_float_eq(sum.terms[2].coefficient, 4.0, 0.001);
    cr_assert_float_eq(sum.terms[1].coefficient, 3.3, 0.001);
    cr_assert_float_eq(sum.terms[0].coefficient, 0.99, 0.001);

    free(addend_1.terms);
    free(addend_2.terms);
    free(sum.terms);

    printf("Test addPolynomials passed!\n");
}

