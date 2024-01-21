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
        if (difference > TOLERANCE) {
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

Test(base_suite, add_same_polynomial_degree_1, .description="Adds two polynomials that have the same number of degree but different number of terms.") {
    Polynomial addend_1, addend_2, actual_sum, expected_sum;

    initializePolynomial(&addend_1, 2);
    setCoefficient(&addend_1, 2, -18342.124);
    setCoefficient(&addend_1, 1, 1.23456789);
    setCoefficient(&addend_1, 0, 0.000001);

    initializePolynomial(&addend_2, 2);
    setCoefficient(&addend_2, 2, 0.01);
    setCoefficient(&addend_2, 0, 10000);

    actual_sum = addPolynomials(&addend_1, &addend_2);
    initializePolynomial(&expected_sum, 2);
    setCoefficient(&expected_sum, 2, -18342.114);
    setCoefficient(&expected_sum, 1, 1.23456789);
    setCoefficient(&expected_sum, 0, 10000.000001);

    cr_assert(arePolynomialsEqual(&actual_sum, &expected_sum), "Polynomials are not equal.");
    free(addend_1.terms);
    free(addend_2.terms);
    free(actual_sum.terms);
    free(expected_sum.terms);
}

Test(base_suite, add_same_polynomial_terms_1, .description="Adds two polynomials that have the same number of terms but different number of degree.") {
    Polynomial addend_1, addend_2, actual_sum, expected_sum;

    initializePolynomial(&addend_1, 3);
    setCoefficient(&addend_1, 3, 5.65);
    setCoefficient(&addend_1, 2, -1.353);
    setCoefficient(&addend_1, 1, 12.45);
    setCoefficient(&addend_1, 0, -3.123);

    initializePolynomial(&addend_2, 4);
    setCoefficient(&addend_2, 4, 44.13);
    setCoefficient(&addend_2, 2, 2.0);
    setCoefficient(&addend_2, 1, 3.55);
    setCoefficient(&addend_2, 0, 0.0);

    actual_sum = addPolynomials(&addend_1, &addend_2);
    initializePolynomial(&expected_sum, 4);
    setCoefficient(&expected_sum, 4, 44.13);
    setCoefficient(&expected_sum, 3, 5.65);
    setCoefficient(&expected_sum, 2, 0.647);
    setCoefficient(&expected_sum, 1, 16);
    setCoefficient(&expected_sum, 0, -3.123);

    cr_assert(arePolynomialsEqual(&actual_sum, &expected_sum), "Polynomials are not equal.");
    free(addend_1.terms);
    free(addend_2.terms);
    free(actual_sum.terms);
    free(expected_sum.terms);
}

Test(base_suite, add_diff_polynomial_1, .description="Adds two polynomials that have different number of degree and terms.") {
    Polynomial addend_1, addend_2, actual_sum, expected_sum;

    initializePolynomial(&addend_1, 3);
    setCoefficient(&addend_1, 3, 5.1245);
    setCoefficient(&addend_1, 2, 99);
    setCoefficient(&addend_1, 1, 12.45);
    setCoefficient(&addend_1, 0, -3.123);

    initializePolynomial(&addend_2, 4);
    setCoefficient(&addend_2, 2, 1);
    setCoefficient(&addend_2, 1, 3.551);
    setCoefficient(&addend_2, 0, 6.666);

    actual_sum = addPolynomials(&addend_1, &addend_2);
    initializePolynomial(&expected_sum, 4);
    setCoefficient(&expected_sum, 4, 0);
    setCoefficient(&expected_sum, 3, 5.1245);
    setCoefficient(&expected_sum, 2, 100);
    setCoefficient(&expected_sum, 1, 16.001);
    setCoefficient(&expected_sum, 0, 3.543);

    cr_assert(arePolynomialsEqual(&actual_sum, &expected_sum), "Polynomials are not equal.");
    free(addend_1.terms);
    free(addend_2.terms);
    free(actual_sum.terms);
    free(expected_sum.terms);
}

Test(base_suite, subtract_same_polynomial_1, .description="Subtracts two polynomials that have the same number of degree and term.") {
    Polynomial minuend, subtrahend, actual_difference, expected_difference;

    initializePolynomial(&minuend, 2);
    setCoefficient(&minuend, 2, 5.65);
    setCoefficient(&minuend, 1, 3.45);
    setCoefficient(&minuend, 0, 0.123);

    initializePolynomial(&subtrahend, 2);
    setCoefficient(&subtrahend, 2, 0.65);
    setCoefficient(&subtrahend, 1, -3.45);
    setCoefficient(&subtrahend, 0, 0.456);

    actual_difference = subtractPolynomials(&minuend, &subtrahend);
    initializePolynomial(&expected_difference, 2);
    setCoefficient(&expected_difference, 2, 5);
    setCoefficient(&expected_difference, 1, 6.9);
    setCoefficient(&expected_difference, 0, -0.333);

    cr_assert(arePolynomialsEqual(&actual_difference, &expected_difference), "Polynomials are not equal.");
    free(minuend.terms);
    free(subtrahend.terms);
    free(actual_difference.terms);
    free(expected_difference.terms);
}

Test(base_suite, subtract_same_polynomial_degree_1, .description="Subtracts two polynomials that have the same number of degree but different number of terms.") {
    Polynomial minuend, subtrahend, actual_difference, expected_difference;

    initializePolynomial(&minuend, 2);
    setCoefficient(&minuend, 2, 5.65);
    setCoefficient(&minuend, 0, 0.123);

    initializePolynomial(&subtrahend, 2);
    setCoefficient(&subtrahend, 2, 0.65);
    setCoefficient(&subtrahend, 1, -3.45);
    setCoefficient(&subtrahend, 0, 0.456);

    actual_difference = subtractPolynomials(&minuend, &subtrahend);
    initializePolynomial(&expected_difference, 2);
    setCoefficient(&expected_difference, 2, 5);
    setCoefficient(&expected_difference, 1, 3.45);
    setCoefficient(&expected_difference, 0, -0.333);

    cr_assert(arePolynomialsEqual(&actual_difference, &expected_difference), "Polynomials are not equal.");
    free(minuend.terms);
    free(subtrahend.terms);
    free(actual_difference.terms);
    free(expected_difference.terms);
}

Test(base_suite, subtract_same_polynomial_term_1, .description="Subtracts two polynomials that have the same number of terms but different number of degree.") {
    Polynomial minuend, subtrahend, actual_difference, expected_difference;

    initializePolynomial(&minuend, 2);
    setCoefficient(&minuend, 2, 5.65);
    setCoefficient(&minuend, 1, 3.45);
    setCoefficient(&minuend, 0, 0.123);

    initializePolynomial(&subtrahend, 4);
    setCoefficient(&subtrahend, 4, 0.65);
    setCoefficient(&subtrahend, 3, -3.45);
    setCoefficient(&subtrahend, 0, 0.456);

    actual_difference = subtractPolynomials(&minuend, &subtrahend);
    initializePolynomial(&expected_difference, 4);
    setCoefficient(&expected_difference, 4, -0.65);
    setCoefficient(&expected_difference, 3, 3.45);
    setCoefficient(&expected_difference, 2, 5.65);
    setCoefficient(&expected_difference, 1, 3.45);
    setCoefficient(&expected_difference, 0, -0.333);

    cr_assert(arePolynomialsEqual(&actual_difference, &expected_difference), "Polynomials are not equal.");
    free(minuend.terms);
    free(subtrahend.terms);
    free(actual_difference.terms);
    free(expected_difference.terms);
}

Test(base_suite, subtract_same_polynomial_term_2, .description="Subtracts two polynomials that have the same number of terms but different number of degree.") {
    Polynomial minuend, subtrahend, actual_difference, expected_difference;

    initializePolynomial(&minuend, 4);
    setCoefficient(&minuend, 4, 0.65);
    setCoefficient(&minuend, 3, -3.45);
    setCoefficient(&minuend, 0, 0.456);

    initializePolynomial(&subtrahend, 2);
    setCoefficient(&subtrahend, 2, 5.65);
    setCoefficient(&subtrahend, 1, 3.45);
    setCoefficient(&subtrahend, 0, 0.123);

    actual_difference = subtractPolynomials(&minuend, &subtrahend);
    initializePolynomial(&expected_difference, 4);
    setCoefficient(&expected_difference, 4, 0.65);
    setCoefficient(&expected_difference, 3, -3.45);
    setCoefficient(&expected_difference, 2, -5.65);
    setCoefficient(&expected_difference, 1, -3.45);
    setCoefficient(&expected_difference, 0, 0.333);

    cr_assert(arePolynomialsEqual(&actual_difference, &expected_difference), "Polynomials are not equal.");
    free(minuend.terms);
    free(subtrahend.terms);
    free(actual_difference.terms);
    free(expected_difference.terms);
}

Test(base_suite, subtract_diff_polynomial_1, .description="Subtracts two polynomials that have different number of degree and terms.") {
    Polynomial minuend, subtrahend, actual_difference, expected_difference;

    initializePolynomial(&minuend, 2);
    setCoefficient(&minuend, 2, 5.65);
    setCoefficient(&minuend, 1, 3.45);
    setCoefficient(&minuend, 0, 0.123);

    initializePolynomial(&subtrahend, 4);
    setCoefficient(&subtrahend, 4, 0.65);
    setCoefficient(&subtrahend, 0, 0.456);

    actual_difference = subtractPolynomials(&minuend, &subtrahend);
    initializePolynomial(&expected_difference, 4);
    setCoefficient(&expected_difference, 4, -0.65);
    setCoefficient(&expected_difference, 3, 0);
    setCoefficient(&expected_difference, 2, 5.65);
    setCoefficient(&expected_difference, 1, 3.45);
    setCoefficient(&expected_difference, 0, -0.333);

    cr_assert(arePolynomialsEqual(&actual_difference, &expected_difference), "Polynomials are not equal.");
    free(minuend.terms);
    free(subtrahend.terms);
    free(actual_difference.terms);
    free(expected_difference.terms);
}

Test(base_suite, multiply_same_polynomial_1, .description="Multiplies two polynomials that have the same number of degree and term.") {
    Polynomial multiplicand, multiplier, actual_product, expected_product;

    initializePolynomial(&multiplicand, 2);
    setCoefficient(&multiplicand, 2, 1.4);
    setCoefficient(&multiplicand, 1, -3.45);
    setCoefficient(&multiplicand, 0, -3.123);

    initializePolynomial(&multiplier, 2);
    setCoefficient(&multiplier, 2, 0.3);
    setCoefficient(&multiplier, 1, 2);
    setCoefficient(&multiplier, 0, -0.456);

    actual_product = multiplyPolynomials(&multiplicand, &multiplier);
    initializePolynomial(&expected_product, 4);
    setCoefficient(&expected_product, 4, 0.42);
    setCoefficient(&expected_product, 3, 1.765);
    setCoefficient(&expected_product, 2, -8.4753);
    setCoefficient(&expected_product, 1, -4.6728);
    setCoefficient(&expected_product, 0, 1.424088);

    cr_assert(arePolynomialsEqual(&actual_product, &expected_product), "Polynomials are not equal.");
    free(multiplicand.terms);
    free(multiplier.terms);
    free(actual_product.terms);
    free(expected_product.terms);
}

Test(base_suite, multiply_same_polynomial_degree_1, .description="Multiplies two polynomials that have the same number of degree but different number of terms.") {
    Polynomial multiplicand, multiplier, actual_product, expected_product;

    initializePolynomial(&multiplicand, 2);
    setCoefficient(&multiplicand, 2, 1.4);
    setCoefficient(&multiplicand, 1, -3.45);
    setCoefficient(&multiplicand, 0, -3.123);

    initializePolynomial(&multiplier, 2);
    setCoefficient(&multiplier, 2, 0.3);
    setCoefficient(&multiplier, 1, 2);

    actual_product = multiplyPolynomials(&multiplicand, &multiplier);
    initializePolynomial(&expected_product, 4);
    setCoefficient(&expected_product, 4, 0.42);
    setCoefficient(&expected_product, 3, 1.765);
    setCoefficient(&expected_product, 2, -7.8369);
    setCoefficient(&expected_product, 1, -6.246);
    setCoefficient(&expected_product, 0, 0);

    cr_assert(arePolynomialsEqual(&actual_product, &expected_product), "Polynomials are not equal.");
    free(multiplicand.terms);
    free(multiplier.terms);
    free(actual_product.terms);
    free(expected_product.terms);
}

Test(base_suite, multiply_same_polynomial_term_1, .description="Multiplies two polynomials that have the same number of terms but different number of degree.") {
    Polynomial multiplicand, multiplier, actual_product, expected_product;

    initializePolynomial(&multiplicand, 2);
    setCoefficient(&multiplicand, 2, 1.4);
    setCoefficient(&multiplicand, 0, -3.123);

    initializePolynomial(&multiplier, 1);
    setCoefficient(&multiplier, 1, 2);
    setCoefficient(&multiplier, 0, -0);

    actual_product = multiplyPolynomials(&multiplicand, &multiplier);
    initializePolynomial(&expected_product, 3);
    setCoefficient(&expected_product, 3, 2.8);
    setCoefficient(&expected_product, 2, 0);
    setCoefficient(&expected_product, 1, -6.246);
    setCoefficient(&expected_product, 0, 0);

    cr_assert(arePolynomialsEqual(&actual_product, &expected_product), "Polynomials are not equal.");
    free(multiplicand.terms);
    free(multiplier.terms);
    free(actual_product.terms);
    free(expected_product.terms);
}

Test(base_suite, multiply_diff_polynomial_1, .description="Multiplies two polynomials that have different number of degree and terms.") {
    Polynomial multiplicand, multiplier, actual_product, expected_product;

    initializePolynomial(&multiplicand, 2);
    setCoefficient(&multiplicand, 2, 1.4);
    setCoefficient(&multiplicand, 1, -3.45);
    setCoefficient(&multiplicand, 0, -3.123);

    initializePolynomial(&multiplier, 0);
    setCoefficient(&multiplier, 0, -0.456);

    actual_product = multiplyPolynomials(&multiplicand, &multiplier);
    initializePolynomial(&expected_product, 2);
    setCoefficient(&expected_product, 2, -0.6384);
    setCoefficient(&expected_product, 1, 1.5732);
    setCoefficient(&expected_product, 0, 1.424088);

    cr_assert(arePolynomialsEqual(&actual_product, &expected_product), "Polynomials are not equal.");
    free(multiplicand.terms);
    free(multiplier.terms);
    free(actual_product.terms);
    free(expected_product.terms);
}

Test(base_suite, divide_same_polynomial_1, .description="Divides two polynomials that have the same number of degree and term.") {
    Polynomial dividend, divisor, actual_quotient, expected_quotient, actual_remainder, expected_remainder;
    Polynomial *solution;

    initializePolynomial(&dividend, 2);
    setCoefficient(&dividend, 2, 1.4);
    setCoefficient(&dividend, 1, -3.45);
    setCoefficient(&dividend, 0, -3.123);

    initializePolynomial(&divisor, 2);
    setCoefficient(&divisor, 2, 0.3);
    setCoefficient(&divisor, 1, 2);
    setCoefficient(&divisor, 0, -0.456);

    solution = dividePolynomials(&dividend, &divisor);

    actual_quotient = solution[0];
    initializePolynomial(&expected_quotient, 0);
    setCoefficient(&expected_quotient, 0, 4.666666);

    actual_remainder = solution[1];
    initializePolynomial(&expected_remainder, 2);
    setCoefficient(&expected_remainder, 2, 0);
    setCoefficient(&expected_remainder, 1, -12.783333);
    setCoefficient(&expected_remainder, 0, -0.995);

    cr_assert(arePolynomialsEqual(&actual_quotient, &expected_quotient), "Polynomials are not equal.");
    cr_assert(arePolynomialsEqual(&actual_remainder, &expected_remainder), "Remainders are not equal.");
    free(dividend.terms);
    free(divisor.terms);
    free(actual_quotient.terms);
    free(expected_quotient.terms);
    free(actual_remainder.terms);
    free(expected_remainder.terms);
    free(solution);
}

Test(base_suite, divide_same_polynomial_degree_1, .description="Divides two polynomials that have the same number of degree but different number of terms.") {
    Polynomial dividend, divisor, actual_quotient, expected_quotient, actual_remainder, expected_remainder;
    Polynomial *solution;

    initializePolynomial(&dividend, 2);
    setCoefficient(&dividend, 2, 1.4);
    setCoefficient(&dividend, 1, -3.45);
    setCoefficient(&dividend, 0, -3.123);

    initializePolynomial(&divisor, 2);
    setCoefficient(&divisor, 2, 0.3);
    setCoefficient(&divisor, 1, -2);

    solution = dividePolynomials(&dividend, &divisor);

    actual_quotient = solution[0];
    initializePolynomial(&expected_quotient, 0);
    setCoefficient(&expected_quotient, 0, 4.666666);

    actual_remainder = solution[1];
    initializePolynomial(&expected_remainder, 2);
    setCoefficient(&expected_remainder, 2, 0);
    setCoefficient(&expected_remainder, 1, 5.883333);
    setCoefficient(&expected_remainder, 0, -3.123);

    cr_assert(arePolynomialsEqual(&actual_quotient, &expected_quotient), "Polynomials are not equal.");
    cr_assert(arePolynomialsEqual(&actual_remainder, &expected_remainder), "Remainders are not equal.");
    free(dividend.terms);
    free(divisor.terms);
    free(actual_quotient.terms);
    free(expected_quotient.terms);
    free(actual_remainder.terms);
    free(expected_remainder.terms);
    free(solution);
}

Test(base_suite, divide_same_polynomial_term_1, .description="Divides two polynomials that have the same number of terms but different number of degree.") {
    Polynomial dividend, divisor, actual_quotient, expected_quotient, actual_remainder, expected_remainder;
    Polynomial *solution;

    initializePolynomial(&dividend, 3);
    setCoefficient(&dividend, 3, 1.4);
    setCoefficient(&dividend, 1, -3.45);
    setCoefficient(&dividend, 0, -3.123);

    initializePolynomial(&divisor, 2);
    setCoefficient(&divisor, 2, 0.3);
    setCoefficient(&divisor, 1, -2);
    setCoefficient(&divisor, 0, 0.24);

    solution = dividePolynomials(&dividend, &divisor);

    actual_quotient = solution[0];
    initializePolynomial(&expected_quotient, 1);
    setCoefficient(&expected_quotient, 1, 4.666666);
    setCoefficient(&expected_quotient, 0, 31.111111);

    actual_remainder = solution[1];
    initializePolynomial(&expected_remainder, 3);
    setCoefficient(&expected_remainder, 3, 0);
    setCoefficient(&expected_remainder, 2, 0);
    setCoefficient(&expected_remainder, 1, 57.652222);
    setCoefficient(&expected_remainder, 0, -10.589666);

    cr_assert(arePolynomialsEqual(&actual_quotient, &expected_quotient), "Polynomials are not equal.");
    cr_assert(arePolynomialsEqual(&actual_remainder, &expected_remainder), "Remainders are not equal.");
    free(dividend.terms);
    free(divisor.terms);
    free(actual_quotient.terms);
    free(expected_quotient.terms);
    free(actual_remainder.terms);
    free(expected_remainder.terms);
    free(solution);
}

Test(base_suite, divide_diff_polynomial_1, .description="Divides two polynomials that have different number of degree and terms.") {
    Polynomial dividend, divisor, actual_quotient, expected_quotient, actual_remainder, expected_remainder;
    Polynomial *solution;

    initializePolynomial(&dividend, 3);
    setCoefficient(&dividend, 3, 12);
    setCoefficient(&dividend, 1, 0);
    setCoefficient(&dividend, 0, 0);

    initializePolynomial(&divisor, 1);
    setCoefficient(&divisor, 1, 4);
    setCoefficient(&divisor, 0, 0);

    solution = dividePolynomials(&dividend, &divisor);

    actual_quotient = solution[0];
    initializePolynomial(&expected_quotient, 2);
    setCoefficient(&expected_quotient, 2, 3);
    setCoefficient(&expected_quotient, 1, 0);
    setCoefficient(&expected_quotient, 0, 0);

    actual_remainder = solution[1];
    initializePolynomial(&expected_remainder, 3);
    setCoefficient(&expected_remainder, 3, 0);
    setCoefficient(&expected_remainder, 2, 0);
    setCoefficient(&expected_remainder, 1, 0);
    setCoefficient(&expected_remainder, 0, 0);

    cr_assert(arePolynomialsEqual(&actual_quotient, &expected_quotient), "Polynomials are not equal.");
    cr_assert(arePolynomialsEqual(&actual_remainder, &expected_remainder), "Remainders are not equal.");
    free(dividend.terms);
    free(divisor.terms);
    free(actual_quotient.terms);
    free(expected_quotient.terms);
    free(actual_remainder.terms);
    free(expected_remainder.terms);
    free(solution);
}

