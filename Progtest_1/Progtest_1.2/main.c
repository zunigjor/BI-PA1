// Libraries
#include <stdio.h>
#include <math.h>
#include <float.h>
// Definitions
#define MULTIPLICATIVE_CONSTANT 64
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
/* Func checks if triangle can be constructed.
 * first three parameters are triangle params
 * fourth param is type of triangle
 * Finds the two shorter sides. Compares them to the longest.
 * Returns 0 if successful.
 */
int triang_can_be_constructed(double, double, double, int);
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
/*
 * Scans what type of triangle were working with.
 * 0 = Error
 * 1 = SSS
 * 2 = SUS
 * 3 = USU
 */
int scan_triang_type();
/*
 * Checks if SSS triang can be constructed.
 * 0 := success
 * 1 := fail
 */
int check_SSS(double, double, double);
/*
 * Checks if SUS triang can be constructed.
 * 0 := success
 * 1 := fail
 */
int check_SUS(double, double, double);
/*
 * Checks if USU triang can be constructed.
 * 0 := success
 * 1 := fail
 */
int check_USU(double, double, double);
/*
 * Calculates the lenght of opposite side length using tangens.
 */
void transform_SUS_to_SSS(double,double*,double);
/*
 *
 */
void transform_USU_to_SSS(double*,double,double*);
// Main
int main(void) {
    // Input management
    printf("Trojuhelnik #1:\n");
    double t1a, t1b, t1c;
    if (load_nums(&t1a, &t1b, &t1c) != 0){
        return 1;
    }
    printf("Trojuhelnik #2:\n");
    double t2a, t2b, t2c;
    if (load_nums(&t2a, &t2b, &t2c) != 0){
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
    int type;
    type = scan_triang_type();
    if (type == 0){
        printf("Nespravny vstup.\n");
        return 1; // input error
    }
    // scanf input
    // comapre if smaller than 0
    if(scanf("%lf %lf %lf", &a, &b, &c) != 3 || a < 0 || b < 0 || c < 0){
        printf("Nespravny vstup.\n");
        return 1; // input error
    }
    // check if close enough to 0
    if (dbl_comp(a,0.0) == 0 || dbl_comp(b,0.0) == 0 || dbl_comp(c,0.0) == 0){
        printf("Nespravny vstup.\n");
        return 1; // input error
    }
    // check if triangle can be constructed
    if (triang_can_be_constructed(a,b,c,type) != 0){
        return 1; // cannot be constructed
    }
    if (type == 2) {
        transform_SUS_to_SSS(a,&b,c);
    } else if (type == 3){
        transform_USU_to_SSS(&a,b,&c);
    }
    // load into given params
    *A = a;
    *B = b;
    *C = c;
    return 0;
}
int dbl_comp(double a, double b){
    if ( fabs(a - b) <= MULTIPLICATIVE_CONSTANT * DBL_EPSILON * (fabs(a) + fabs(b))){
        return 0;
    }
    return 1;
}
int triang_can_be_constructed(double a, double b, double c, int type){
    if (type == 1){
        return check_SSS(a,b,c);
    } else if (type == 2){
        return check_SUS(a,b,c);
    } else if (type == 3){
        return check_USU(a,b,c);
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
    return (dbl_comp(t1a,t2a) == 0 &&
            dbl_comp(t1b,t2b) == 0 &&
            dbl_comp(t1c,t2c) == 0) ? 0 : 1;
}
void swap(double * A, double * B){
    double a=*A,b=*B;
    *B = a;
    *A = b;
}
int scan_triang_type(){
    char x1,x2,x3;
    if (scanf(" %c%c%c",&x1,&x2,&x3) != 3 ||
            (x1 != 'S' && x1 != 'U') ||
            (x2 != 'S' && x2 != 'U') ||
            (x3 != 'S' && x3 != 'U')){
        return 0; // input error
    }
    if (x1 == 'S' && x2 == 'S' && x3 == 'S'){
        return 1; // SSS
    }
    if (x1 == 'S' && x2 == 'U' && x3 == 'S'){
        return 2; // SUS
    }
    if (x1 == 'U' && x2 == 'S' && x3 == 'U'){
        return 3; // USU
    }
    return 0; // input error
}
int check_SSS(double a, double b, double c){
    double max,od1,od2;
    (a > b) ? (max = a, od1 = b) : (max = b, od1 = a);
    (max > c) ? (od2 = c) : (od2 = max, max = c);
    if (od1 + od2 < max || dbl_comp(od1+od2,max) == 0){
        printf("Vstup netvori trojuhelnik.\n");
        return 1; // cannot be constructed
    }
    return 0; // can be constructed
}
int check_SUS(double a, double b, double c){
    if (b < 180){
        return 0; // can be constructed
    }
    printf("Nespravny vstup.\n");
    return 1; // cannot be constructed
}
int check_USU(double a, double b, double c){
    if (a+c < 180){
        return 0; // can be constructed
    }
    if (dbl_comp(a,180.0) == 0 || dbl_comp(c,180.0) == 0){
        printf("Nespravny vstup.\n");
        return 1;
    }
    printf("Vstup netvori trojuhelnik.\n");
    return 1; // cannot be constructed
}
void transform_SUS_to_SSS(double A,double* B, double C){
    // B is angle
    double a=A,b=*B,c=C;
    double b2;
    double newB;
    // law of cosines
    // b^2 = a^2 + c^2 - 2 * a * c * cos(beta)
    b2 = (a * a) + (c * c) - (2 * a * c * cos(b * M_PI / 180.0));
    newB = sqrt(b2);
    *B=newB;
}
void transform_USU_to_SSS(double* A,double B,double* C){
    // A and C are angles
    double angle_A=*A,b=B,angle_C=*C;
    double angle_B = 180.0 - angle_A - angle_C;
    double side_a = b * ( sin(angle_A * M_PI / 180.0) / sin(angle_B * M_PI / 180.0) );
    double side_c = b * ( sin(angle_C * M_PI / 180.0) / sin(angle_B * M_PI / 180.0) );
    *A = side_a;
    *C = side_c;
}