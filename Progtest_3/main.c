#ifndef __PROGTEST__

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <assert.h>

#endif /* __PROGTEST__ */
# define DBL_PRECISION 2
/*
 * Compares double values with set precision DBL_PRECISION
 */
int dblcomp(double a, double b){
    if (fabs(a - b) <= DBL_PRECISION *  DBL_EPSILON * (fabs(a)+fabs(b))){
        return 1;
    }
    return 0;
}
/*
 * returns maximum of two doubles
 */
double max(double a, double b){
    if (dblcomp(a,b) || a > b)
        return a;
    return b;
}
/*
 * returns minimum of two doubles
 */
double min(double a, double b){
    if (dblcomp(a,b) || a < b)
        return a;
    return b;
}
/*
 * swaps values
 */
void swap(double * aPtr, double * bPtr){
    double tmp = *aPtr;
    *aPtr = *bPtr;
    *bPtr = tmp;
}
/*
 * returns length between points
 */
double ptsDistance(double Ax, double Ay, double Bx, double By){
    double dist2 = ((Bx-Ax)*(Bx-Ax))+((By-Ay)*(By-Ay));
    return sqrt(dist2);
}
/*
 * Evaluates if point given is inside of rectangle with precision given by almostEqual function
 * First pair of params is the point in the left bottom corner
 * Second pair of params is the point in the right top corner
 * return
 * 1 := is inside
 * 0 := is outside
 */
int isInRectang(double rx1, double ry1, double rx2, double ry2, double  Ax, double  Ay) {
    return (((Ax > rx1) || dblcomp(Ax, rx1)) &&
            ((Ax < rx2) || dblcomp(Ax, rx2)) &&
            ((Ay > ry1) || dblcomp(Ay, ry1)) &&
            ((Ay < ry2) || dblcomp(Ay, ry2))) ? 1 : 0;
}
/*
 * Compares if the point is on the line segment.
 * Does not check if point is on the line.
 * return
 * 1 := is inside segment
 * 0 := is NOT inside segment
 */
int isInSegment(double Ax, double Ay,
                double Bx, double By,
                double  X, double  Y){
    int XmoreEqualMinX = (min(Ax,Bx) < X || dblcomp(min(Ax,Bx),X)),
        XlessEqualMaxX = (X < max(Ax,Bx) || dblcomp(max(Ax,Bx),X)),
        YmoreEqualMinY = (min(Ay,By) < Y || dblcomp(min(Ay,By),Y)),
        YlessEqualMaxY = (Y < max(Ay,By) || dblcomp(max(Ay,By),Y));
    if (XlessEqualMaxX && XmoreEqualMinX && YlessEqualMaxY && YmoreEqualMinY)
        return 1;
    return 0;
}

/*
 * Calculates the intersection of two line segments given by their end points.
 *  first line given by first four params (two pairs of (x,y))
 *  second line given by another four params
 *  last two params save x and y coordinates of the intersection
 *  retrun
 *  1 := intersection found
 *  0 := parrallel lines or collinear lines
 */
int findIntersection(double Ax, double Ay, double Bx, double By,  //// first line
                     double Cx, double Cy, double Dx, double Dy,  //// second line
                     double *X, double *Y){                       //// return parameters
    //// line equations for finding line intersections
    double A1 = By - Ay,
           B1 = Ax - Bx,
           C1 = A1 * Ax + B1 * Ay,
           A2 = Dy - Cy,
           B2 = Cx - Dx,
           C2 = A2 * Cx + B2 * Cy,
           denom = A1 * B2 - A2 * B1;
    //// if the denominator is 0 the lines are either paralel or collinear
    if(dblcomp(denom,0)){
        return 0;
    }
    //// used for both X and Y axis
    double intersectX = (C1*B2 - C2*B1)/denom,
           intersectY = (A1*C2 - A2*C1)/denom;
    //// check if found intersection is in both line segments
    int line1SegmentRatioLogic = isInSegment(Ax,Ay,Bx,By,intersectX,intersectY);
    int line2SegmentRatioLogic = isInSegment(Cx,Cy,Dx,Dy,intersectX,intersectY);
    //// if intersection exists assign it to return params and return 1
    if (line1SegmentRatioLogic && line2SegmentRatioLogic){
        *X = intersectX;
        *Y = intersectY;
        return 1;
    }
    return 0;
}


int clipLine(double rx1, double ry1, //// souradnice jednoho rohu obdelniku
             double rx2, double ry2, //// souradnice druheho rohu obdelniku
             double *axPtr, double *ayPtr, //// input&output // input := souradnice koncoveho bodu usecky // output := souradnice orezane usecky
             double *bxPtr, double *byPtr) {//// input&output // input := souradnice koncoveho bodu usecky // output := souradnice orezane usecky
    //// Place rx1,ry1 in left bottom corner and rx2,ry2 in right top corner
    if (rx1 > rx2) { swap(&rx1, &rx2); }
    if (ry1 > ry2) { swap(&ry1, &ry2); }
    //// Assign to local variables
    double Ax = *axPtr, Ay = *ayPtr,
           Bx = *bxPtr, By = *byPtr;
    //// find if points are inside of given rectangle
    int is_A_in_rect = isInRectang(rx1, ry1, rx2, ry2, Ax, Ay),
        is_B_in_rect = isInRectang(rx1, ry1, rx2, ry2, Bx, By);
    //// Calculate height and width
    double obdSirka = rx2 - rx1,
           obdVyska = ry2 - ry1;
    //// All four rectangle sides have to be checked for intersections
    //// From the amount of points inside and outside of the rectangle we can assume
    //// How many intersections are to be found
    //// If both points are inside of the rectangle no changes are needed and return 1
    //// If one of the points is inside and the other is outside
    ////    there is one intersection point which gets assigned to the outside's point cordinates
    //// If both points are outside two things can happen
    ////    1) The line fomred by the two points does not cross the rectangle
    ////            In this case we return 0
    ////    2) The line crosses the rectangle and both values need to be changed
    ////            In this case one or two intersections will be found
    ////            -o- If only one intersection is found it means the line touches the rectangle on a single edge point
    ////            -o- If two intersections are found we have to assign the right values to the right points
    ////                    This is done by measuring the distance between one of the intersection points
    ////                    and line segment end points. The closer point get reassigned to the intersection values
    //// Calculate the rest of the rectangle's edge points                  //// r3 ..... r2
    double rx3 = rx1, ry3 = ry1 + obdVyska,                                 ////    |   |
           rx4 = rx1 + obdSirka, ry4 = ry1,                                 //// r1 ..... r4
           x1 = 0, y1 = 0, //// first intersection point
           x2 = 0, y2 = 0; //// second itnersection point (needed in case of two intersections found)
    //// intersections found
    int intersecitonsFound = 0;
    if (is_A_in_rect == 1 && is_B_in_rect == 1) { //// 0 intersections, no changes needed
        return 1;
    } else if (is_A_in_rect == 0 && is_B_in_rect == 1) { //// 1 intersection, A gets changed
        if (findIntersection(Ax,Ay,Bx,By,rx1,ry1,rx3,ry3,&x1,&y1)){*axPtr = x1; *ayPtr = y1;return 1;} //// |AB| X |r1r3|
        if (findIntersection(Ax,Ay,Bx,By,rx1,ry1,rx4,ry4,&x1,&y1)){*axPtr = x1; *ayPtr = y1;return 1;} //// |AB| X |r1r4|
        if (findIntersection(Ax,Ay,Bx,By,rx2,ry2,rx3,ry3,&x1,&y1)){*axPtr = x1; *ayPtr = y1;return 1;} //// |AB| X |r2r3|
        if (findIntersection(Ax,Ay,Bx,By,rx2,ry2,rx4,ry4,&x1,&y1)){*axPtr = x1; *ayPtr = y1;return 1;} //// |AB| X |r2r4|
    } else if (is_A_in_rect == 1 && is_B_in_rect == 0) { //// 1 intersection, B gets changed
        if (findIntersection(Ax,Ay,Bx,By,rx1,ry1,rx3,ry3,&x1,&y1)){*bxPtr = x1; *byPtr = y1;return 1;} //// |AB| X |r1r3|
        if (findIntersection(Ax,Ay,Bx,By,rx1,ry1,rx4,ry4,&x1,&y1)){*bxPtr = x1; *byPtr = y1;return 1;} //// |AB| X |r1r4|
        if (findIntersection(Ax,Ay,Bx,By,rx2,ry2,rx3,ry3,&x1,&y1)){*bxPtr = x1; *byPtr = y1;return 1;} //// |AB| X |r2r3|
        if (findIntersection(Ax,Ay,Bx,By,rx2,ry2,rx4,ry4,&x1,&y1)){*bxPtr = x1; *byPtr = y1;return 1;} //// |AB| X |r2r4|
    } else if (is_A_in_rect == 0 && is_B_in_rect == 0){ //// 0 (both outside, line doesnt cross rect) or 2 intersections(both outside, line crosses rect)
        if (findIntersection(Ax,Ay,Bx,By,rx1,ry1,rx3,ry3,&x1,&y1)){intersecitonsFound++;} //// |AB| X |r1r3| saving to x1,y1
        if (intersecitonsFound == 0){
            if (findIntersection(Ax,Ay,Bx,By,rx1,ry1,rx4,ry4,&x1,&y1)){intersecitonsFound++;} //// |AB| X |r1r4| saving to x1,y1
        } else if (intersecitonsFound == 1){
            if (findIntersection(Ax,Ay,Bx,By,rx1,ry1,rx4,ry4,&x2,&y2)){ //// |AB| X |r1r4| saving to x2,y2
                double distA_XY1 = ptsDistance(Ax,Ay,x1,y1),
                       distB_XY1 = ptsDistance(Bx,By,x1,y1);
                if (dblcomp(distA_XY1,distB_XY1)){ //// the distance from the intersection points is almost equal
                    *axPtr = x1; *ayPtr = y1;
                    *bxPtr = x2; *byPtr = y2;
                } else if (distA_XY1 < distB_XY1){ //// the distance between point A and intersection X1 is smaller than dist(B,X1)
                    *axPtr = x1; *ayPtr = y1; //// A is closer to intersect point X1 therefore A1 gets reassigned to X1
                    *bxPtr = x2; *byPtr = y2; //// B gets reassigned to X2
                } else {                      //// B is closer
                    *axPtr = x2; *ayPtr = y2; //// A gets assigned X2
                    *bxPtr = x1; *byPtr = y1; //// B gets assigned X1
                }
                return 1; //// Two intersections were found
            }
        }
        if (intersecitonsFound == 0){ //// two lines checked and no intersections found
            if (findIntersection(Ax,Ay,Bx,By,rx2,ry2,rx3,ry3,&x1,&y1)){ //// |AB| X |r2r3| saving to x1,y1 && has to find an intersection or there are none
                intersecitonsFound++;
            } else {
                return 0;  //// third line has been checked and no intersections found
            }
        } else if(intersecitonsFound == 1){ //// two lines checked and one intersection found
            if (findIntersection(Ax,Ay,Bx,By,rx2,ry2,rx3,ry3,&x2,&y2)){ //// |AB| X |r2r3| saving to x2,y2
                double distA_XY1 = ptsDistance(Ax,Ay,x1,y1), distB_XY1 = ptsDistance(Bx,By,x1,y1); //// again find distances and assign to right points
                if (dblcomp(distA_XY1,distB_XY1)){*axPtr = x1; *ayPtr = y1;*bxPtr = x2; *byPtr = y2;
                } else if (distA_XY1 < distB_XY1){*axPtr = x1; *ayPtr = y1;*bxPtr = x2; *byPtr = y2;
                } else {*axPtr = x2; *ayPtr = y2;*bxPtr = x1; *byPtr = y1;}
                return 1;
            }
        }
        if (intersecitonsFound == 1) { //// three lines checked and one intersection found
            if (findIntersection(Ax,Ay,Bx,By,rx2,ry2,rx4,ry4,&x2,&y2)){ //// |AB| X |r2r4| saving to x2,y2
                double distA_XY1 = ptsDistance(Ax,Ay,x1,y1), distB_XY1 = ptsDistance(Bx,By,x1,y1);   //// second intersection found
                if (dblcomp(distA_XY1,distB_XY1)){*axPtr = x1; *ayPtr = y1;*bxPtr = x2; *byPtr = y2; //// calc dist and assign to right points
                } else if (distA_XY1 < distB_XY1){*axPtr = x1; *ayPtr = y1;*bxPtr = x2; *byPtr = y2;
                } else {*axPtr = x2; *ayPtr = y2;*bxPtr = x1; *byPtr = y1;}
                return 1;
            } else {  //// Three lines checked and only one intersection found
                *axPtr = x1; *ayPtr = y1; //// Assign both lines to the same intersection point
                *bxPtr = x1; *byPtr = y1;
                return 1;
            }
        }
    }
    return 0; //// smth went wrong I guess
}

#ifndef __PROGTEST__

int almostEqual(double a, double b){
    if (fabs(a - b) <= DBL_PRECISION * DBL_EPSILON * (fabs(a)+fabs(b))){return 1;}
    return 0;
}

int main(void) {
    double x1, y1, x2, y2;
/*
    x1 = 60;
    y1 = 40;
    x2 = 70;
    y2 = 50;
    assert (clipLine(10, 20, 90, 100, &x1, &y1, &x2, &y2)
            && almostEqual(x1, 60)
            && almostEqual(y1, 40)
            && almostEqual(x2, 70)
            && almostEqual(y2, 50));

    x1 = 60;
    y1 = 40;
    x2 = 70;
    y2 = 50;
    assert (clipLine(90, 100, 10, 20, &x1, &y1, &x2, &y2)
            && almostEqual(x1, 60)
            && almostEqual(y1, 40)
            && almostEqual(x2, 70)
            && almostEqual(y2, 50));

    x1 = 0;
    y1 = 50;
    x2 = 20;
    y2 = 30;
    assert (clipLine(90, 100, 10, 20, &x1, &y1, &x2, &y2)
            && almostEqual(x1, 10)
            && almostEqual(y1, 40)
            && almostEqual(x2, 20)
            && almostEqual(y2, 30));

    x1 = 0;
    y1 = 30;
    x2 = 120;
    y2 = 150;
    assert (clipLine(10, 20, 90, 100, &x1, &y1, &x2, &y2)
            && almostEqual(x1, 10)
            && almostEqual(y1, 40)
            && almostEqual(x2, 70)
            && almostEqual(y2, 100));

    x1 = -10;
    y1 = -10;
    x2 = -20;
    y2 = -20;
    assert (!clipLine(10, 20, 90, 100, &x1, &y1, &x2, &y2));

    x1 = 0;
    y1 = 30;
    x2 = 20;
    y2 = 10;
    assert (clipLine(10, 20, 90, 100, &x1, &y1, &x2, &y2)
            && almostEqual(x1, 10)
            && almostEqual(y1, 20)
            && almostEqual(x2, 10)
            && almostEqual(y2, 20));
*/
    x1 = 0;
    y1 = 0.3553;
    x2 = 10.45;
    y2 = 0;
    assert(clipLine(0.95, 0.323, 1, 1, &x1, &y1, &x2, &y2));
    assert(almostEqual(x1, 0.95));
    assert(almostEqual(y1, 0.323));
    assert(almostEqual(x2, 0.95));
    assert(almostEqual(y2, 0.323));

    return 0;
}

#endif /* __PROGTEST__ */