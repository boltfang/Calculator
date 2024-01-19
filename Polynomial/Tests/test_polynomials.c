#include "test_polynomials.h"
 
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

void expect_normal_exit(int status) {
    cr_expect_eq(status, 0, "The program did not exit normally (status = 0x%x).\n", status);
}

void expect_error_exit(int actual_status, int expected_status) {
    cr_expect_eq(WEXITSTATUS(actual_status), expected_status,
		 "The program exited with status %d instead of %d.\n", WEXITSTATUS(actual_status), expected_status);
}

void expect_no_valgrind_errors(int status) {
    cr_expect_neq(WEXITSTATUS(status), 37, "Valgrind reported errors -- see %s", test_log_outfile);
}

TestSuite(base_suite, .timeout=TEST_TIMEOUT);
TestSuite(base_valgrind, .timeout=TEST_TIMEOUT, .disabled=false);
TestSuite(base_invalid_args, .timeout=TEST_TIMEOUT);

bool arePolynomialsEqual(const Polynomial *polynomial_1, const Polynomial *polynomial_2) {
    if (polynomial_1->degree != polynomial_2->degree) {
        return false;
    }

    for (int i = 0; i <= polynomial_1->degree; i++) {
        double difference = fabs(polynomial_1->terms[i].coefficient - polynomial_2->terms[i].coefficient);
        if (difference > EPSILON) {
            return false;
        }
    }

    return true;
}

Test(base_suite, add_same_polynomial_1, .description="Adds two polynomials that have the same number of degree and term.") {
    Polynomial addend_1, addend_2, actual_sum, expected_sum;

    initializePolynomial(&addend_1, 3);
    setCoefficient(&addend_1, 3, 3.2);
    setCoefficient(&addend_1, 2, -12.3);
    setCoefficient(&addend_1, 1, 3.45);
    setCoefficient(&addend_1, 0, 3.123);

    initializePolynomial(&addend_2, 3);
    setCoefficient(&addend_2, 3, 4.73);
    setCoefficient(&addend_2, 2, 0.3);
    setCoefficient(&addend_2, 1, -3.45);
    setCoefficient(&addend_2, 0, 0.456);

    actual_sum = addPolynomials(&addend_1, &addend_2);
    initializePolynomial(&expected_sum, 3);
    setCoefficient(&expected_sum, 3, 7.93);
    setCoefficient(&expected_sum, 2, -12);
    setCoefficient(&expected_sum, 1, 0);
    setCoefficient(&expected_sum, 0, 3.579);

    cr_assert(arePolynomialsEqual(&actual_sum, &expected_sum), "Polynomials are not equal.");
    free(addend_1.terms);
    free(addend_2.terms);
    free(actual_sum.terms);
    free(expected_sum.terms);
}

