#include <math.h>
#include <stdio.h>

// f(x) = x^3 - x - 1  
// f(x) = 2x^3 - 2x - 5  
// f(x) = sqrt(12)
// f(x) = 3*sqrt(48)
// f(x) = 2*e^x − 2x − 3
// f(x) = 3x + sin(x) - exp(x)
// f(x) = exp(-x) - 3log(x)

double f1(double x) { return (x * x * x - x - 1); }
double f2(double x) { return 2 * x * x * x - 2 * x - 5; }
double f3(double x) { return x - sqrt(12); }
double f4(double x) { return x - 3 * sqrt(48); }
double f5(double x) { return 2 * exp(x) - 2 * x - 3; }
double f6(double x) { return 3 * x + sin(x) - exp(x); }
double f7(double x) { return exp(-x) - 3 * log(x); }

/*
    Generic function for bijection method.
    def: bisect(double initial_x1, double initial_x2, double tolerance, double (*function)());
    Return the root (till passed precision or till n iterations) using bisection method with x_1 and x_2 as initial guess.
*/
double bisect(double x_1, double x_2, double tolerance, double (*F)(double)) {
    double x0, x1, x2;
    double f0, f1, f2;
    double p = 0.00001;

    x1 = x_1;
    x2 = x_2;
    p = tolerance;

    f1 = F(x1);
    f2 = F(x2);

    if (f1 * f2 >= 0 || x1 == x2) {
        printf("Not valid initial values.\n");
        return 0;
    }

    int count;
    while (1) {
        double temp = x0;
        x0 = (x1 + x2) / 2.0;
        f0 = F(x0);
        // printf("x0: %lf, x1: %lf, x2: %lf, f0: %lf, f1: %lf, f2: %lf \n", x0, x1, x2, f0, f1, f2);
        if (f0 == 0) { // root found
            return x0;
        } else if (f0 * f1 < 0) { // root is b/w x0 and x1
            x2 = x0;
            f2 = f0;
        } else { // root is b/w x0 and x2
            x1 = x0;
            f1 = f0;
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
    printf("1. f(x) = x^3 - x - 1  \n");
    printf("2. f(x) = 2x^3 - 2x - 5  \n");
    printf("3. f(x) = sqrt(12)\n");
    printf("4. f(x) = 3*sqrt(48)\n");
    printf("5. f(x) = 2*e^x − 2x − 3\n");
    printf("6. f(x) = 3x + sin(x) - exp(x)\n");
    printf("7. f(x) = exp(-x) - 3log(x)\n");
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
    case 7:
        func = f7;
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
    while (func(x_1) * func(x_2) > 0 || x_1 == x_2) {
        printf("Enter a valid inital range: ");
        scanf("%lf %lf", &x_1, &x_2);
    }
    double ans = bisect(x_1, x_2, p, func);
    printf("Roots of equation are:: %lf", ans);
    return 0;
}