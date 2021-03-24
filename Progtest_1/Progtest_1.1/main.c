// Libraries
#include <stdio.h>
#include <math.h>
// Definitions
#define DBL_EPS 1e-5
// Program
/* Func that loads triangle sides.
 * Checks input.
 * Checks if triangle can be made.
 * Returns 0 if successful.
 * */
int load_nums(double*, double*, double*);
/* Func that compares two doubles.
 * Returns int 0 if they are close enough (based on DBL_EPS).
 * Otherwise returns 1;
 * */
int dbl_comp(double, double);
/* Funch checks if triangle can be constructed.
 * Finds the two shorter sides. Compares them to the longest.
 * Returns 0 if successful.
 */
int triang_can_be_constructed(double, double, double);
/*
 * Sort triangle sides.
 * Low to high
 */
void triang_sort(double*, double*, double*);
/*
 * Calculates if triangles are similar.
 * Length of each
 */
int triang_similar(double, double, double, double, double, double);
/*
 * Calculates if triangles are equal.
 */
int triang_equal(double, double, double, double, double, double);
/*
 * Swaps numbers
 */
void swap(double*, double*);
// Main
int main(void) {
    // Input management
    printf("Trojuhelnik #1:\n");
    double t1a, t1b, t1c;
    int err;
    err = load_nums(&t1a, &t1b, &t1c);
    if ( err == 1){
        printf("Nespravny vstup.\n");
        return 1;
    }
    if (err == 2){
        printf("Strany netvori trojuhelnik.\n");
        return 1;
    }
    printf("Trojuhelnik #2:\n");
    double t2a, t2b, t2c;
    err = load_nums(&t2a, &t2b, &t2c);
    if ( err == 1){
        printf("Nespravny vstup.\n");
        return 1;
    } else if (err == 2){
        printf("Strany netvori trojuhelnik.\n");
        return 1;
    }
    // Sort
    triang_sort(&t1a,&t1b,&t1c);
    triang_sort(&t2a,&t2b,&t2c);
    // Calculations
    if (triang_equal(t1a, t1b, t1c, t2a, t2b, t2c) == 0){
        printf("Trojuhelniky jsou shodne.\n");
        return 0;
    }
    if (triang_similar(t1a, t1b, t1c, t2a, t2b, t2c) == 0){
        printf("Trojuhelniky nejsou shodne, ale jsou podobne.\n");
        return 0;
    }
    printf("Trojuhelniky nejsou shodne ani podobne.\n");
    return 0;
}

int load_nums(double * A, double * B, double * C){
    double a,b,c;
    // scanf input
    // comapre if smaller than 0
    if(scanf("%lf %lf %lf", &a, &b, &c) != 3 || a < 0 || b < 0 || c < 0){
        return 1;
    }
    // check if close enough to 0
    if (dbl_comp(a,0.0) == 0 || dbl_comp(b,0.0) == 0 || dbl_comp(c,0.0) == 0){
        return 1;
    }
    // check if triangle can be constructed
    if (triang_can_be_constructed(a,b,c) != 0){
        return 2;
    }
    // load into given params
    *A = a;
    *B = b;
    *C = c;
    return 0;
}

int dbl_comp(double a, double b){
    if ( fabs(a - b) <= DBL_EPS * ( fabs(a) + fabs(b) ) ){
        return 0;
    }
    return 1;
}

int triang_can_be_constructed(double a, double b, double c){
    double max,od1,od2;
    (a > b) ? (max = a, od1 = b) : (max = b, od1 = a);
    (max > c) ? (od2 = c) : (od2 = max, max = c);
    if (od1 + od2 < max || dbl_comp(od1+od2,max) == 0){
        return 1;
    }
    return 0;
}

void triang_sort(double * A, double * B, double * C){
    double a=*A, b=*B, c=*C;
    if (a > c) swap(&a,&c);
    if (a > b) swap(&a,&b);
    if (b > c) swap(&b,&c);
    *A = a;
    *B = b;
    *C = c;
}

int triang_similar(double t1a, double t1b, double t1c, double t2a, double t2b, double t2c){
    return (dbl_comp(t1a/t2a,t1b/t2b) == 0 &&
            dbl_comp(t1a/t2a,t1c/t2c) == 0 &&
            dbl_comp(t1b/t2b,t1c/t2c) == 0) ? 0 : 1;
}

int triang_equal(double t1a, double t1b, double t1c, double t2a, double t2b, double t2c){
    return (t1a == t2a && t1b == t2b && t1c == t2c) ? 0 : 1;
}

void swap(double * A, double * B){
    double a=*A,b=*B;
    *B = a;
    *A = b;
}