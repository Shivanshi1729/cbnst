#include <math.h>
#include <stdio.h>

double f1(double x) { return (x * cos(x / (x - 2))); }
double f2(double x) { return x * x - exp(-2 * x) - 1 / x; }
double f3(double x) { return exp(x * x - 1 + 10 * sin(2 * x) - 5); }
double f4(double x) { return exp(x) - 3 * x * x; }
double f5(double x) { return tan(x) - x - 1; }
double f6(double x) { return sin(2 * x) - exp(x - 1); }

double regulafalsi(double x_1, double x_2, double tolerance, double (*F)(double)) {
    double x0, x1, x2;
    double f0, f1, f2;
    double p;

    x1 = x_1;
    x2 = x_2;
    p = tolerance;

    if (F(x1) * F(x2) >= 0 || x1 == x2) {
        printf("Not valid initial values.\n");
        return 0;
    }

    int count;
    while (1) {
        double temp = x0;
        x0 = (x1 * F(x2) - x2 * F(x1)) / (F(x2) - F(x1));
        // printf("%lf \t| %lf \t| %lf \t| %lf\n", x1, x0, x2, F(x0));
        if (F(x0) == 0) { // root found
            return x0;
        } else if (F(x0) * F(x1) < 0) { // root is b/w x0 and x1
            x2 = x0;
        } else { // root is b/w x0 and x2
            x1 = x0;
        }
        if (fabs(temp - x0) < p) {
            return x0;
        }
    }
    return 0;
}

int main() {

    double (*func)(double);
    printf("Choose the function:: \n");
    printf("1. f(x) = (x * cos(x / (x - 2)))\n");
    printf("2. f(x) = x * x - exp(-2 * x) - 1 / x\n");
    printf("3. f(x) = exp(x * x - 1 + 10 * sin(2 * x) - 5)\n");
    printf("4. f(x) = exp(x) - 3 * x * x\n");
    printf("5. f(x) = tan(x) - x - 1\n");
    printf("6. f(x) = sin(2 * x) - exp(x - 1)\n");
    int c;
    scanf("%d", &c);

    switch (c) {
    case 1:
        func = f1;
        break;
    case 2:
        func = f2;
        break;
    case 3:
        func = f3;
        break;
    case 4:
        func = f4;
        break;
    case 5:
        func = f5;
        break;
    case 6:
        func = f6;
        break;
    default:
        return 0;
        break;
    }

    int noDigits = 4;
    double p = 0.5 * pow(10.0, -noDigits);
    double x_1 = 0;
    double x_2 = 0;
    printf("Enter the initial values:\n");
    while ( x_1 == x_2 || func(x_1) * func(x_2) > 0) {
        printf("Enter a valid inital range: ");
        scanf("%lf %lf", &x_1, &x_2);
    }
    double ans = regulafalsi(x_1, x_2, p, func);
    printf("Roots of equation are:: %lf", ans);
    return 0;
}