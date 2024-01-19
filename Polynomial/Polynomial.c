#include "polynomial.h"

void initializePolynomial(Polynomial *polynomial, int degree) {
    if (degree < 0) {
        fprintf(stderr, "ERROR IN INITIALIZATION: INVALID DEGREE");
        exit(EXIT_FAILURE);
    }

    polynomial->degree = degree;
    polynomial->terms = (Term*)malloc(sizeof(Term) * (degree + 1));
    if (polynomial->terms == NULL) {
        fprintf(stderr, "ERROR IN INITIALIZATION: MEMORY ALLOCATION FAILURE");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i <= degree; i++) {
        polynomial->terms[i].coefficient = 0.0;
        polynomial->terms[i].exponent = i;
    }
}

void setCoefficient(Polynomial *polynomial, int exponent, double coefficient) {
    if (exponent < 0 || exponent > polynomial->degree) {
        fprintf(stderr, "ERROR IN SETTING COEFFICIENT: INVALID EXPONENT");
        exit(EXIT_FAILURE);
    }
    polynomial->terms[exponent].coefficient = coefficient;
}

void copyPolynomial(const Polynomial *source, Polynomial *destination) {
    initializePolynomial(destination, source->degree);
    for (int i = 0; i < source->degree; i++) {
        setCoefficient(destination, i, source->terms[i].coefficient);
    }
}

Polynomial addPolynomials(const Polynomial *addend_1, const Polynomial *addend_2) {
    Polynomial sum;
    int maxDegree = (addend_1->degree > addend_2->degree) ? addend_1->degree : addend_2->degree;
    initializePolynomial(&sum, maxDegree);

    for (int i = 0; i <= maxDegree; i++) {
        double coefficient_1 = (i <= addend_1->degree) ? addend_1->terms[i].coefficient : 0.0;
        double coefficient_2 = (i <= addend_2->degree) ? addend_2->terms[i].coefficient : 0.0;
        setCoefficient(&sum, i, coefficient_1 + coefficient_2);
    }

    return sum;
}

Polynomial subtractPolynomials(const Polynomial *minuend, const Polynomial *subtrahend) {
    Polynomial difference;
    int maxDegree = (minuend->degree > subtrahend->degree) ? minuend->degree : subtrahend->degree;
    initializePolynomial(&difference, maxDegree);

    for (int i = 0; i <= maxDegree; i++) {
        double coefficient_1 = (i <= minuend->degree) ? minuend->terms[i].coefficient : 0.0;
        double coefficient_2 = (i <= subtrahend->degree) ? subtrahend->terms[i].coefficient : 0.0;
        setCoefficient(&difference, i, coefficient_1 - coefficient_2);
    }

    return difference;
}

Polynomial multiplyPolynomials(const Polynomial *multiplicand, const Polynomial *multiplier) {
    Polynomial product;
    int maxDegree = multiplicand->degree + multiplier->degree;
    initializePolynomial(&product, maxDegree);

    for (int i = 0; i <= multiplicand->degree; i++) {
        for (int j = 0; j <= multiplier->degree; j++) {
            double coefficient_1 = multiplicand->terms[i].coefficient;
            double coefficient_2 = multiplicand->terms[j].coefficient;
            setCoefficient(&product, i + j, coefficient_1 * coefficient_2);
        }
    }

    return product;
}

Polynomial *dividePolynomials(const Polynomial *dividend, const Polynomial *divisor) {
    if (divisor->degree == 0 && divisor->terms[0].coefficient == 0.0) {
        fprintf(stderr, "ERROR IN DIVISION: INVALID DIVISOR");
        exit(EXIT_FAILURE);
    }
    Polynomial quotient, remainder;
    int maxDegree = dividend->degree - divisor->degree;
    initializePolynomial(&quotient, maxDegree);
    copyPolynomial(dividend, &remainder);

    for (int i = dividend->degree; i >= divisor->degree; i--) {
        double coefficient_1 = divisor->terms[divisor->degree].coefficient;
        double coefficient_2 = remainder.terms[i].coefficient;
        setCoefficient(&quotient, i - divisor->degree, coefficient_1 / coefficient_2);

        for (int j = 0; j <= divisor->degree; j++) {
            remainder.terms[i - j].coefficient -= coefficient_2 * divisor->terms[j].coefficient;
        }
    }

    Polynomial *result = (Polynomial*)malloc(sizeof(Polynomial) * 2);
    if (result == NULL) {
        fprintf(stderr, "ERROR IN DIVISION: MEMORY ALLOCATION FAILURE");
        exit(EXIT_FAILURE);
    }

    result[0] = quotient;
    result[1] = remainder;
    return result;
}