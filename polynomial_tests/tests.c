#include <stdio.h>
#include <assert.h>
#include "polynomial.c"

void test_add_same_degree_and_term() {
    Polynomial addend_1, addend_2, sum;

    initializePolynomial(&addend_1, 2);
    setCoefficient(&addend_1, 2, 2.0);
    setCoefficient(&addend_1, 1, -1.3);
    setCoefficient(&addend_1, 0, 0.3);

    initializePolynomial(&addend_2, 2);
    setCoefficient(&addend_2, 2, 5.1);
    setCoefficient(&addend_2, 1, 2.0);
    setCoefficient(&addend_2, 0, 0.69);

    sum = addPolynomials(&addend_1, &addend_2);

    assert(sum.degree = 2);
    assert(sum.terms[2].coefficient = 7.1);
    assert(sum.terms[1].coefficient = 0.7);
    assert(sum.terms[0].coefficient = 0.99);

    free(addend_1.terms);
    free(addend_2.terms);
    free(sum.terms);

    printf("TEST: ADD SAME DEGREE AND SAME NUMBER OF TERMS : PASSED");
}

int main() {
    test_add_same_degree_and_term();
    return 0;
}
