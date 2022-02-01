/* 
    Author: Shivanshu
    Description: sample program to solve simultaneous linear equations having only integer
        coefficients.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double stringToDigit(char *a) {
    char *f;
    int n = strlen(a);
    if (n == 0) return 1;
    if (n == 1 && a[0] == '-') return -1;
    if (n == 1 && a[0] == '+') return 1;
    return strtod(a, &f);
}

/* 
    Return Status:: 
    0 - successful parsing
    1 - failed
 */
int praseEq(char *a, double *coeff, int n) {

    char b[100] = {0};
    // trimming and invalid symbol checker
    for (int i = 0, j = 0; i <= strlen(a); i++) {
        if (a[i] == ' ') {
            continue;
        } else if (a[i] >= 'a' + n && a[i] <= 'z') {
            printf("Invalid symbol found %c\n", a[i]);
            return 1;
        } else if ((a[i] < 'a' + n && a[i] >= 'a') || (a[i] <= '9' || a[i] >= '0') || a[i] == '.') {
            b[j] = a[i];
            j++;
        } else {
            printf("Invalid symbol found %c\n", a[i]);
            return 1;
        }
    }
    // printf("String after trimming:: %s\n", b);

    char c[100] = {0};
    int j = 0, i = 0;
    for (i = 0;; i++) {
        if (b[i] == '\0') {
            c[j] == '\0';
            coeff[n] += stringToDigit(c);
            j = 0;
            break;
        }
        if (b[i] == '=') {
            c[j] == '\0';
            coeff[b[i] - 'a'] += stringToDigit(c);
            j = 0;
            memset(c, 0, sizeof c);
        } else if (b[i] < 'a' + n && b[i] >= 'a') {
            c[j] == '\0';
            coeff[b[i] - 'a'] += stringToDigit(c);
            j = 0;
            memset(c, 0, sizeof c);
        } else {
            c[j] = b[i];
            j++;
        }
    }
    return 0;
}

int main() {
    int n;
    printf("Enter the number of equation:: ");
    scanf("%d", &n);

    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    double a[n][n + 1];

    // Input
    char eq[100] = {0}, *lf;

    printf("Enter the equations\n");

    double *aa = (double *)malloc(sizeof(double) * (n + 1));
    for (int i = 0; i < n; i++) {
        int c;

        // scanf("%s", eq);
        fgets(eq, sizeof(eq), stdin);
        if ((lf = strchr(eq, '\n')) != NULL) *lf = '\0';

        printf("n :: %d \t eq :: %s \n", n, eq);

        if (praseEq(eq, aa, n) == 1) {
            printf("Error!!!\n");
            exit(1);
        } else {
            printf("Parsing eq:: ");
            for (int j = 0; j < n + 1; j++) {
                printf("%lf ", aa[j]);
                a[i][j] = aa[j];
            }
            printf("\n");
        }
        memset(aa, 0.0, sizeof(double) * (n + 1));
    }

    // Printing input matrix
    printf("Input Matrix:: \n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n + 1; j++) {
            if (j == n) printf(" | ");
            printf("%.2lf ", a[i][j]);
        }
        printf("\n");
    }
    // converting lower triangular matrix
    for (int i = 0; i < n; i++) {
        if (a[i][i] == 0) {
            for (int j = i + 1; j < n; j++) {
                if (a[j][i] != 0) {
                    for (int k = 0; k < n + 1; k++) {
                        double temp = a[i][k];
                        a[i][k] = a[j][k];
                        a[j][k] = temp;
                    }
                }
            }
        }
        if (a[i][i] == 0) {
            fprintf(stderr, "Given system of eq do not have a Unique Solution.");
            return 1;
        }
        // make the first element 1
        double temp = a[i][i];
        for (int j = i; j < n + 1; j++) {
            a[i][j] = a[i][j] / temp;
        }
        for (int j = i + 1; j < n; j++) {
            if (a[j][i] != 0) {
                double div = a[j][i] / a[i][i];
                for (int k = i; k < n + 1; k++) {
                    a[j][k] = a[j][k] - a[i][k] * div;
                }
            }
        }
    }
    // printf("Upper Triangular:: \n");
    // for (int i = 0; i < n; i++) {
    //     for (int j = 0; j < n + 1; j++) {
    //         if (j == n) printf(" | ");
    //         printf("%.2lf ", a[i][j]);
    //     }
    //     printf("\n");
    // }
    for (int i = n - 1; i >= 0; i--) {
        for (int j = i - 1; j >= 0; j--) {
            // R_j <- R_j - (a[j][i] / a[i][i]) * R_i
            double div = a[j][i] / a[i][i];
            for (int k = 0; k < n + 1; k++) {
                a[j][k] = a[j][k] - div * a[i][k];
            }
        }
    }
    // printf("Diagonal :: \n");
    // for (int i = 0; i < n; i++) {
    //     for (int j = 0; j < n + 1; j++) {
    //         if (j == n) printf(" | ");
    //         printf("%.2lf ", a[i][j]);
    //     }
    //     printf("\n");
    // }
    double x[n]; // array to store solutions
    for (int i = 0; i < n; i++) {
        x[i] = a[i][n];
    }
    printf("Solutions: \n");
    for (int i = 0; i < n; i++) {
        printf("%c = %lf\n", 'a' + i, x[i]);
    }
    return 0;
}