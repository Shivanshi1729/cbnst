#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

#include "./C/pbPlots.h"
#include "./C/supportLib.h"

typedef struct polynomial {
    int n;
    double *coeff;
    double *degree;
} polynomial;

/* Calculate the value of plynomial at a given value. */
double calPoly(polynomial *p, double x) {
    double result = 0;
    for (int i = 0; i < p->n; i++) {
        result += p->coeff[i] * pow(x, p->degree[i]);
    }
    return result;
}

/* Parse a polynomial and return a array of decree and coefficients, and the length of those array len */
int parsePolynomial(char *temp, double *degree, double *coeff, int *len) {
    int n = strlen(temp);

    char eq[1000] = {0};
    int j = 0;
    for (int i = 0; i < n; i++) {
        char c = temp[i];
        if (c == ' ') {
            continue;
        } else if ((c >= '0' && c <= '9') || c == '.' || c == 'x' || c == 'X' || c == '+' || c == '-' || c == '^') {
            eq[j++] = temp[i];
        } else {
            printf("\n Error while parsing character { %c } \n", c);
            return -1;
        }
    }

    n = strlen(eq);
    char eq2[1000] = {0};
    j = 0;
    for (int i = 0; i < n; i++) {
        if (i == 0 && j == 0 && eq[i] == 'x') {
            eq2[j++] = '1';
            i--;
        } else if ((eq[i] == 'x') && (i == n - 1 || eq[i + 1] == '+' || eq[i + 1] == '-')) {
            eq2[j++] = eq[i];
            eq2[j++] = '^';
            eq2[j++] = '1';
        } else if ((eq[i] == '+' || eq[i] == '-') && (eq[i + 1] == 'x')) {
            eq2[j++] = eq[i];
            eq2[j++] = '1';
        } else {
            eq2[j++] = eq[i];
        }
    }
    // eq2[j] == '\0';
    int j0 = 0, j1 = 0;
    char *f = eq2, *s;
    for (int i = 0; i < strlen(eq2);) {
        coeff[j0++] = strtod(f, &s);
        i += s - f;
        if (eq2[i] == 'x') {
            i += 2;
            f = eq2 + i;
            degree[j1++] = strtod(f, &s);
            i += s - f;
            f = eq2 + i;
        } else {
            degree[j1++] = 0;
            f = s;
        }
    }
    *len = j1;
    // printf("\ndegree:: ");
    // for (int i = 0; i < j0; i++)
    //     printf("%lf ", degree[i]);
    // printf("\ncoeff :: ");
    // for (int i = 0; i < j1; i++)
    //     printf("%lf ", coeff[i]);

    return 0;
}

char *displayPolynomial(polynomial *p) {
    char *ch1 = malloc(sizeof(char) * 1000);
    char ch[1000] = {0};
    int i;
    for (i = 0; i < p->n; i++) {
        if (p->coeff[i] == 0) {
            continue;
        } else if (p->coeff[i] == 1) {
            if (p->degree[i] == 1) {
                sprintf(ch, "(x)+");
            } else {
                sprintf(ch, "(x^%.2lf)+", p->degree[i]);
            }
        } else if (p->degree[i] == 0) {
            sprintf(ch, "(%.2lf)+", p->coeff[i]);
        } else if (p->degree[i] == 1) {
            sprintf(ch, "(%.2lfx)+", p->coeff[i]);
        } else {
            sprintf(ch, "(%.2lfx^%.2lf)+", p->coeff[i], p->degree[i]);
        }
        strcat(ch1, ch);
    }
    ch1[strlen(ch1) - 1] = '\0';
    return ch1;
}

polynomial *getDerivative(polynomial *p) {
    polynomial *der = malloc(sizeof(polynomial));
    double *degree = (double *)malloc(sizeof(double) * p->n);
    double *coeff = (double *)malloc(sizeof(double) * p->n);
    der->degree = degree;
    der->coeff = coeff;
    der->n = p->n;
    for (int i = 0; i < p->n; i++) {
        if (p->degree[i] == 0) {
            der->coeff[i] = 0;
            der->degree[i] = 0;
        } else {
            der->coeff[i] = p->degree[i] * p->coeff[i];
            der->degree[i] = p->degree[i] - 1;
        }
    }
    return der;
}

double bisection(polynomial *p, double x0, double x1) {

    double c = 0;
    // calculating tolerance
    int noDigits = 4;
    double t = 0.5 * pow(10.0, -noDigits);

    double x_prev = 0;

    printf("x0\t| c\t| x1\t| f(c)\n");
    while (1) {
        x_prev = c;
        c = (x0 + x1) / 2;
        printf("%lf\t| %lf\t| %lf\t| %lf\n", x0, c, x1, calPoly(p, c));
        if (calPoly(p, c) == 0) {
            printf("Result is :: %lf", c);
            break;
        } else if (calPoly(p, x0) * calPoly(p, c) < 0) {
            x1 = c;
        } else {
            x0 = c;
        }
        if (fabs(c - x_prev) <= t) {
            printf("Result is :: %lf  within %lf tolerance", c, t);
            break;
        }
    }
    return c;
}

double newtonRaphson(polynomial *p, polynomial *p0, double xn) {

    double xn0;
    xn0 = xn;
    double slope_lim = 0.00001;
    // calculating tolerance
    int noDigits = 4;
    double t = 0.5 * pow(10.0, -noDigits);

    printf("x_i\t| f(x_i)\n");
    while (1) {
        if (fabs(calPoly(p0, xn0)) < slope_lim) {
            printf("Slope is too small. (stuck at some local maxima or minima) Stoping execution....");
            exit(0);
        }
        xn = xn0 - (calPoly(p, xn0) / calPoly(p0, xn0));
        printf("%lf\t| %lf\n", xn, calPoly(p, xn));
        if (calPoly(p, xn) == 0) {
            printf("Result :: %lf", xn);
            break;
        } else if (fabs(xn0 - xn) < t) {
            printf("Result:: %lf within %lf tolerance.", xn, t);
            break;
        }
        xn0 = xn;
    }
    return xn;
}

double regulaFalsi(polynomial *p, double x0, double x1) {
    double c = 0;

    // calculating tolerance
    int noDigits = 4;
    double t = 0.5 * pow(10.0, -noDigits);
    double x_prev = 0;
    printf("x0\t| c\t| x1\t| f(c)\n");
    while (1) {
        x_prev = c;
        c = (x0 * calPoly(p, x1) - x1 * calPoly(p, x0)) / (calPoly(p, x1) - calPoly(p, x0));
        printf("%lf\t| %lf\t| %lf\t| %lf\n", x0, c, x1, calPoly(p, c));
        if (calPoly(p, c) == 0) {
            printf("Result is :: %lf", c);
            break;
        } else if (calPoly(p, x0) * calPoly(p, c) < 0) {
            x1 = c;
        } else {
            x0 = c;
        }
        if (fabs(c - x_prev) < t) {
            printf("Result is :: %lf  within %lf tolerance", c, t);
            break;
        }
    }

    return c;
}

wchar_t * charTowchar_t(char *c){
    int n = strlen(c);
    wchar_t *ch = malloc(sizeof(wchar_t) * (n+40));
    swprintf(ch, n+20, L"f(x) = %hs", c);
    return ch;
}

void makePlot(polynomial *p, double x0, double x1, double root, char *name) {

    int n = 50;

    double x[n];
    double y[n];

    double max_y = DBL_MIN;
    double min_y = DBL_MAX;
    
    double h = (x1 - x0) / (double)n;
    for (int i = 0; i < n; i++) {
        x[i] = x0 + i * h;
        y[i] = calPoly(p, x[i]);
        if(y[i] > max_y) max_y = y[i];
        if(y[i] < min_y) min_y = y[i];
    }

    max_y++;
    min_y--;

    double xr[2] = {root, root};
    double yr[2] = {max_y, min_y};
    // printf("\n============================");
    // printf("\nValues for the plotter:: ");
    // printf("\n x\t y");
    // for (int i = 0; i < n; i++) {
    //     printf("\n%.2lf\t%.2lf", x[i], y[i]);
    // }
    // printf("\n============================");

    /* creating scatter plot of polynomial */
    ScatterPlotSeries *series = GetDefaultScatterPlotSeriesSettings();
    series->xs = x;
    series->xsLength = n;
    series->ys = y;
    series->ysLength = n;
    series->linearInterpolation = true;
    series->lineType = L"dashed";
    series->lineTypeLength = wcslen(series->lineType);
    series->lineThickness = 2;
    series->color = GetGray(0.3);

    /* creating straight line at the root */
    ScatterPlotSeries *series1 = GetDefaultScatterPlotSeriesSettings();
    series1->xs = xr;
    series1->xsLength = 2;
    series1->ys = yr;
    series1->ysLength = 2;
    series1->linearInterpolation = true;
    series1->lineType = L"solid";
    series1->lineTypeLength = wcslen(series1->lineType);
    series1->lineThickness = 2;
    series1->color = CreateRGBColor(0, 1, 0);

    /* creating scatter plot settings  */
    ScatterPlotSettings *settings = GetDefaultScatterPlotSettings();
    settings->width = 800;
    settings->height = 600;
    settings->autoBoundaries = true;
    settings->autoPadding = true;
    char *c = displayPolynomial(p);
    wchar_t *cc = charTowchar_t(c);
    // settings->title = L"hello";
    settings->title = cc;
    settings->titleLength = wcslen(cc);
    settings->xLabel = L"X axis";
    settings->xLabelLength = wcslen(settings->xLabel);
    settings->yLabel = L"Y axis";
    settings->yLabelLength = wcslen(settings->yLabel);
    ScatterPlotSeries *s[] = {series, series1};
    settings->scatterPlotSeries = s;
    settings->scatterPlotSeriesLength = 2;

    RGBABitmapImageReference *canvasReference = CreateRGBABitmapImageReference();
    DrawScatterPlotFromSettings(canvasReference, settings);

    size_t length;
    double *pngdata = ConvertToPNG(&length, canvasReference->image);
    WriteToFile(pngdata, length, name);
    DeleteImage(canvasReference->image);
}

int main() {

    printf("Enter the function::\n");

    char eq[1000] = {0}, *lf;
    fgets(eq, sizeof(eq), stdin);
    if ((lf = strchr(eq, '\n')) != NULL) *lf = '\0';

    double degree[100] = {0.0}, coeff[100] = {0.0};
    int len = 0;
    if (parsePolynomial(eq, degree, coeff, &len) != 0) {
        printf("\nError in polynomial.....\n");
        exit(1);
    }

    polynomial *p = malloc(sizeof(polynomial));
    p->n = len;
    p->degree = degree;
    p->coeff = coeff;

    printf("\n Polynomial:: ||f(x) = %s|| \n", displayPolynomial(p));

    // menu
    printf("\n=======================\n");
    printf("1. Bisection\n");
    printf("2. Regula falsi\n");
    printf("3. Newton's Raphsons\n");
    printf("0. Exit\n");
    printf("Enter your choice:: ");

    int ch, c;
    scanf("%d", &ch);

    switch (ch) {
    case 1: {
        double x0 = 0, x1 = 0;
        // input range till valid range is found
        while (x0 == x1 || calPoly(p, x0) * calPoly(p, x1) > 0) {
            printf("\nEnter a valid range:: ");
            scanf("%lf %lf", &x0, &x1);
        }
        double root = bisection(p, x0, x1);
        printf("\nRoot is:: %lf\n", root);
        makePlot(p, x0, x1, root, "my_image.png");
        break;
    }
    case 2: {
        double x0 = 0, x1 = 0;
        // input range till valid range is found
        while (x0 == x1 || calPoly(p, x0) * calPoly(p, x1) > 0) {
            printf("\nEnter a valid range:: ");
            scanf("%lf %lf", &x0, &x1);
        }
        double root = regulaFalsi(p, x0, x1);
        printf("\nRoot is:: %lf\n", root);
        makePlot(p, x0, x1, root, "my_image.png");
        break;
    }
    case 3: {
        double x0;
        printf("\nEnter the initial guess:: ");
        scanf("%lf", &x0);

        polynomial *p1 = getDerivative(p);
        printf("\nDerivative:: ||f'(x) = %s||\n", displayPolynomial(p1));

        double root = newtonRaphson(p, p1, x0);
        printf("\nRoot:: %lf\n", root);
        double start = ((x0 <= root) ? x0 : root) - 1;
        double end = ((x0 >= root) ? x0 : root) + 1;
        makePlot(p, start, end, root, "my_image.png");
        free(p1);
        break;
    }
    default:
        break;
    }

    free(p);
    return 0;
}