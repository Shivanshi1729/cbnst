#include <math.h>
#include <stdio.h>

double f1(double x) { return -cos(x + 3.141 / 4.0) + 0.8; }
double f2(double x) { return x * exp(-x); }
double f3(double x) { return pow(x, 1.0 / 3); }
double f4(double x) { return x * log(x) - 1; }
double f5(double x) { return tan(x) - 4 * x; }

double f1d(double x) { return sin(x + 3.141 / 4); }
double f2d(double x) { return exp(-x) - x * exp(-x); }
double f3d(double x) { return 1 / (3 * pow(x, 2.0 / 3.0)); }
double f4d(double x) { return log(x) + 1; }
double f5d(double x) { return ((1 / cos(x)) * (1 / cos(x)) - 4); }

void newton(double (*f)(double), double (*f0)(double)) {
    double xn, xn0;

    // calculating tolerance
    int noDigits = 6;
    double t = 0.5 * pow(10.0, -noDigits);

    // input
    printf("Enter initial approximation:: ");
    scanf("%lf", &xn);
    xn0 = xn;

    printf("x_i\t| f(x_i)\n");
    while (1) {
        xn = xn0 - (f(xn0) / f0(xn0));
        printf("%lf\t| %lf\n", xn, f(xn));
        if (f(xn) == 0) {
            printf("Result :: %lf", xn);
            break;
        } else if (fabs(xn0 - xn) < t) {
            printf("Result:: %lf within %lf tolerance.", xn, t);
            break;
        }
        xn0 = xn;
    }
}

int main() {

    double (*func)(double);
    double (*func_der)(double);
    printf("Choose the function:: \n");
    printf("1. f(x) = -cos(x + 3.141 / 4.0) + 0.8\n");
    printf("2. f(x) = x * exp(-x)\n");
    printf("3. f(x) = pow(x, 1 / 3)\n");
    printf("4. f(x) = x * log(x) - 1\n");
    printf("5. f(x) = tan(x) - 4 * x\n");
    int c;
    scanf("%d", &c);

    switch (c) {
    case 1:
        func_der = f1d;
        func = f1;
        break;
    case 2:
        func_der = f2d;
        func = f2;
        break;
    case 3:
        func_der = f3d;
        func = f3;
        break;
    case 4:
        func_der = f4d;
        func = f4;
        break;
    case 5:
        func_der = f5d;
        func = f5;
        break;
    default:
        return 0;
        break;
    }
    newton(func, func_der);
    return 0;
}