#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    double coefficient;
    int exponent;
}Term;

typedef struct {
    int degree;
    Term *terms;
}Polynomial;

void initializePolynomial(Polynomial *polynomial, int degree);
void setCoefficient(Polynomial *polynomial, int exponent, double coefficient);
void copyPolynomial(const Polynomial *source, Polynomial *destination);
Polynomial addPolynomials(const Polynomial *addened_1, const Polynomial *addend_2);
Polynomial subtractPolynomials(const Polynomial *minuend, const Polynomial *subtrahend);
Polynomial multiplyPolynomials(const Polynomial *multiplicand, const Polynomial *multiplier);
Polynomial *dividePolynomials(const Polynomial *dividend, const Polynomial *divisor);

