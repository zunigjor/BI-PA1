/**
 * @file main.c
 * @author zunigjor@fit.cvut.cz
 * @date 2019
 * Solution to Progtest nr.5 Airplanes problem.
 * Input example:
 * [0,0] KLM KL1981
 * [ 2 , 0 ] Wizz Air W67868
 * [5, 0] Etihad Airways ETD26
 * [7, 0] Emirates UAE93P
 * [10, 0] British Airways BA123
 * Expected output:
 * Nejmensi vzdalenost: 2.000000
 * KLM KL1981 - Wizz Air W67868
 * Etihad Airways ETD26 - Emirates UAE93P
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <string.h>
#define MEM_START_LEN 128 /**< Starting size of memory allocation. */
#define MY_DBL_CONSTANT 1024 /**< Multiplicative constant for comparing double values */
/** @struct
 * Point in 2D space structure, holding X and Y values.
 */
 typedef struct S_Point{
     double x; /**< X coordinate. */
     double y; /**< Y coordinate. */
 } s_point;
/** @struct
 * Airplane structure to hold the X,Y coordinates and a name of a plane.
 */
typedef struct S_Airplane{
    s_point coordinates; /**< Point structure holding X and Y coordinates of airplane. */
    char * airplane_Name; /**< Name of airplane. Char array finished with '\0' symbol to make printing easier. */
} s_airplane;
/** @struct
 * Structure to hold the distance and names of airplane pair.
 */
typedef struct S_AirplanePair{
    double distance; /**< Distance between airplane A and B. */
    s_airplane airplane_A; /**< Airplane A. */
    s_airplane airplane_B; /**< Airplane B. */
} s_airplanePair;
/**
 * Function that returns minimum of two double values
 * @param[in] x First number.
 * @param[in] y Second number.
 * @return minimum of two doubles
 */
double dblMin(double x, double y){
    return (x<y)?x:y;
}
/**
 * Function to find whether two double numbers are close enough with relative precision given by
 * the defined DBL constant.
 * @param[in] a First number.
 * @param[in] b Second number.
 * @return 1 if "a==b", 0 if a!=b
 */
int dbl_comp(double a, double b){
    if ( fabs(a - b) <= MY_DBL_CONSTANT * DBL_EPSILON * (fabs(a) + fabs(b))){
        return 1;
    }
    return 0;
}
/**
 * Function to find distance between two points in 2D space.
 * dist = sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y))
 * @param[in] p1 First point.
 * @param[in] p2 Second point.
 */
 double dist(s_point p1, s_point p2) {
    return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}
/**
 * Function to find distance between two airplanes.
 * @param[in] p1 Airplane 1.
 * @param[in] p2 Airplane 2.
 * @return distance between airplanes
 */
double planeDist(s_airplane p1, s_airplane p2){
    return dist(p1.coordinates,p2.coordinates);
}
/**
 * Function which prints the result of the program.
 * @param[in] airplanePairDatabase Pointer to airplane pairs database.
 * @param[in] numberOfPairs Number of pairs.
 */
void printPairs(s_airplanePair * airplanePairDatabase, int numberOfPairs){
    int i=0;
    double minDistance = airplanePairDatabase[i].distance;
    for (i=0; i < numberOfPairs && dbl_comp(airplanePairDatabase[i].distance, minDistance); ++i) {
        printf("%s", airplanePairDatabase[i].airplane_A.airplane_Name);
        printf(" - ");
        printf("%s", airplanePairDatabase[i].airplane_B.airplane_Name);
        printf("\n");
    }
}
/**
 * Function to free the planes database. Frees all plane names and then the whole array of structs.
 * @param[in] airplaneDatabase Pointer to plane database.
 * @param[in] numberOfAirplanes Number of planes in the database.
 */
void freePlanes(s_airplane * airplaneDatabase, int numberOfAirplanes){
    for (int i = 0; i < numberOfAirplanes; ++i) {
        free(airplaneDatabase[i].airplane_Name);
    }
    free(airplaneDatabase);
}
/**
 * Quick sort comparator function for s_airplane structs.
 * Sorted by Y coordinate.
 */
int airplaneCompareY(const void * i_a, const void * i_b){
    const s_airplane * a = (s_airplane*)i_a;
    const s_airplane * b = (s_airplane*)i_b;
    if (a->coordinates.y < b->coordinates.y){
        return 1;
    } else if (a->coordinates.y > b->coordinates.y){
        return -1;
    } else {
        return 0;
    }
}
/**
 * Function which implements binary search for airplanes database.
 * @param[in] airplaneDatabase Pointer to airplane database.
 * @param[in] numberOfPlanes Number of airplanes.
 * @param[in] x X coordinate value.
 * @param[in] y Y coordinate value.
 * @param[in] airplaneName Airplane name.
 * @return Returns 1 if airplane was found, 0 if not found.
 */
int binarySearchAirplane(s_airplane * airplaneDatabase, int numberOfPlanes, double x, double y, char * airplaneName){
    int lo = 0, hi= numberOfPlanes - 1,mid;
    while (lo <= hi){
        mid = (lo + hi)/2;
        if (airplaneDatabase[mid].coordinates.x > x){
            hi = mid - 1;
        } else if (airplaneDatabase[mid].coordinates.x < x) {
            lo = mid +1;
        } else {
            if (airplaneDatabase[mid].coordinates.y > y){
                hi = mid - 1;
            } else if (airplaneDatabase[mid].coordinates.y < y) {
                lo = mid +1;
            } else {
                int nameComp = strcmp(airplaneDatabase[mid].airplane_Name,airplaneName);
                if ( nameComp < 0){
                    hi = mid - 1;
                } else if (nameComp > 0){
                    lo = mid +1;
                } else {
                    return 1;
                }
            }
        }
    }
    return 0;
}
/**
 * Function which returns an evaluation of the expression by which are airplanes added to the database in a sorted way.
 * @param[in] airplaneDatabase Pointer to airplane database.
 * @param[in] i Index in database.
 * @param[in] x X coordinate.
 * @param[in] y Y coordinate.
 * @return Returns 1 if the expression is true, otherwise returns 0.
 */
int insertAirplaneExprEval(s_airplane * airplaneDatabase, int i, double x, double y){
    int insertByX = i >= 0 && airplaneDatabase[i].coordinates.x > x;
    int insertByY = i >= 0 && airplaneDatabase[i].coordinates.x == x && airplaneDatabase[i].coordinates.y > y;
    int insertExpr = insertByX || insertByY;
    return insertExpr;
}
/**
 * Function which inserts the airplane in the right place in the database.
 * The database is primarily sorted by the X value and secondly by the Y value.
 * @param[in] airplaneDatabase Pointer to database of airplanes.
 * @param[in] numberOfPlanes Number of airplanes.
 * @param[in] x Coordinate of new airplane to be added to the database.
 * @param[in] y Coordinate of new airplane to be added to the database.
 * @param[in] airplaneName Name of new airplane to be added to the database.
 */
void insertAirplane(s_airplane * airplaneDatabase, int* numberOfPlanes, double x, double y, char * airplaneName){
    int i;
    if (! binarySearchAirplane(airplaneDatabase, *numberOfPlanes, x, y, airplaneName)){
        for (i = (*numberOfPlanes) - 1; insertAirplaneExprEval(airplaneDatabase, i, x, y) ; i--){
            airplaneDatabase[i+1] = airplaneDatabase[i];
        }
        airplaneDatabase[i+1].coordinates.x = x;
        airplaneDatabase[i+1].coordinates.y = y;
        airplaneDatabase[i+1].airplane_Name = airplaneName;
        (*numberOfPlanes)++;
    }
}
/**
 * Function which removes n characters from the begging of a string.
 * @param[in,out] stringInput Pointer to string (char arr[]).
 * @param[in] numberOfCharsToDrop Number of chars to drop.
 */
void removeChars(char * stringInput, int numberOfCharsToDrop){
    char * stringPointer = stringInput + numberOfCharsToDrop;
    while (*stringPointer){
        *stringInput = *stringPointer;
        ++stringInput;
        ++stringPointer;
    }
    *stringInput = '\0';
}
/**
 * Function that takes care of reading the input.
 * Reads formatted input that is required to be in the following format:
 * [ double , double ] string '\n'
 * Airplanes are divided by newlines.
 * Keeps the array sorted by inserting into right places.
 * @param[in,out] airplaneDatabasePtr Pointer to where the airplane array will be saved.
 * @param[in,out] numberOfPlanesPtr Number of saved airplanes.
 * @return 0 if everything went fine, otherwise returns 1.
 */
int readInput(s_airplane ** airplaneDatabasePtr, int * numberOfPlanesPtr){
    s_airplane * airplaneDatabase = (s_airplane *)malloc(MEM_START_LEN * sizeof(*airplaneDatabase));
    int numberOfPlanes = 0;
    int maxNumOfPlanes = MEM_START_LEN;
    int ret;
    while (1){
        char * airplaneName = (char *)malloc(MEM_START_LEN * sizeof(*airplaneName));
        size_t airplaneNameLen = MEM_START_LEN;
        int charsRead;
        if((charsRead = getline(&airplaneName,&airplaneNameLen,stdin)) == -1){
            free(airplaneName);
            break;
        }
        airplaneName[charsRead-1] = '\0';
        char leftBracket,dash,rightBracket;
        double x,y;
        int numberOfChars;
        ret = sscanf(airplaneName, " %c %lf %c %lf %c %n",&leftBracket,&x,&dash,&y,&rightBracket,&numberOfChars);
        if (ret != 5 || leftBracket != '[' || dash != ',' || rightBracket != ']'){
            free(airplaneName);
            freePlanes(airplaneDatabase, numberOfPlanes);
            return 1;
        }
        if (numberOfPlanes >= maxNumOfPlanes){
            s_airplane * moreAirplanes = (s_airplane*)realloc(airplaneDatabase, maxNumOfPlanes * 2 * sizeof(*moreAirplanes));
            if (moreAirplanes == NULL){
                free(airplaneName);
                freePlanes(airplaneDatabase, numberOfPlanes);
                return 1;
            }
            airplaneDatabase = moreAirplanes;
            maxNumOfPlanes = numberOfPlanes * 2;
        }
        removeChars(airplaneName, numberOfChars);
        insertAirplane(airplaneDatabase, &numberOfPlanes, x, y, airplaneName);
    }
    if(numberOfPlanes < 2){
        freePlanes(airplaneDatabase, numberOfPlanes);
        return 1;
    }
    *numberOfPlanesPtr = numberOfPlanes;
    *airplaneDatabasePtr = airplaneDatabase;
    return 0;
}
/**
 * Function which implements binary search for finding duplicates in database of airplane pairs.
 * @param[in] airplanePairDatabase Pointer to pairs database.
 * @param[in] numberOfPairs Number of pairs in database.
 * @param[in] distance Distance between airplanes.
 * @param[in] airplane_A Airplane A.
 * @param[in] airplane_B Airplane B.
 * @return 1 if pair found, 0 pair not found.
 */
int binarySearchAirplanePair(s_airplanePair * airplanePairDatabase, int numberOfPairs, double distance, s_airplane airplane_A, s_airplane airplane_B){
    int airplaneANameComparison;
    int airplaneBNameComparison;
    int lo = 0, hi= numberOfPairs - 1,mid;
    while (lo <= hi){
        mid = (lo + hi)/2;
        if (airplanePairDatabase[mid].distance > distance){
            hi = mid - 1;
        } else if (airplanePairDatabase[mid].distance < distance) {
            lo = mid +1;
        } else {
            airplaneANameComparison = strcmp(airplane_A.airplane_Name, airplanePairDatabase[mid].airplane_A.airplane_Name);
            if (airplaneANameComparison < 0){
                hi = mid -1;
            } else if (airplaneANameComparison > 0) {
                lo = mid +1;
            } else {
                airplaneBNameComparison = strcmp(airplane_B.airplane_Name, airplanePairDatabase[mid].airplane_B.airplane_Name);
                if (airplaneBNameComparison < 0){
                    hi = mid -1;
                } else if (airplaneBNameComparison > 0) {
                    lo = mid +1;
                } else {
                    if (airplanePairDatabase[mid].airplane_A.coordinates.x > airplane_A.coordinates.x){
                        hi = mid -1;
                    } else if (airplanePairDatabase[mid].airplane_A.coordinates.x < airplane_A.coordinates.x){
                        lo = mid +1;
                    } else {
                        if (airplanePairDatabase[mid].airplane_A.coordinates.y > airplane_A.coordinates.y){
                            hi = mid -1;
                        } else if (airplanePairDatabase[mid].airplane_A.coordinates.y < airplane_A.coordinates.y) {
                            lo = mid +1;
                        } else {
                            if(airplanePairDatabase[mid].airplane_B.coordinates.x > airplane_B.coordinates.x){
                                hi = mid -1;
                            } else if (airplanePairDatabase[mid].airplane_B.coordinates.x < airplane_B.coordinates.x){
                                lo = mid +1;
                            } else {
                                if (airplanePairDatabase[mid].airplane_B.coordinates.y > airplane_B.coordinates.y){
                                    hi = mid-1;
                                } else if (airplanePairDatabase[mid].airplane_B.coordinates.y < airplane_B.coordinates.y){
                                    lo = mid +1;
                                } else {
                                    return 1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}
/**
 * Function which returns an evaluation of the expression by which air pairs of airplanes added to the database in a sorted way.
 * @param[in] airplanePairDatabase Pointer to pairs database.
 * @param[in] i Index in database.
 * @param[in] distance Distance between planes.
 * @param[in] A Airplane A.
 * @param[in] B Airplane B.
 * @return Returns 1 if the expression is true, otherwise returns 0.
 */
int insertAirplanePairExprEval(s_airplanePair * airplanePairDatabase, int i, double distance, s_airplane A, s_airplane B){
    if (i<0)return 0;
    //// insert by distance
    int insertByDistance = i>=0 && airplanePairDatabase[i].distance > distance;
    int equalDistance = i>=0 && airplanePairDatabase[i].distance == distance;
    //// equal distance => insert by plane A name
    int planeANameComp = strcmp(airplanePairDatabase[i].airplane_A.airplane_Name, A.airplane_Name);
    int insertByPlaneAName = equalDistance && planeANameComp > 0;
    //// equal distance and plane A name => insert by plane B name
    int planeBNameComp = strcmp(airplanePairDatabase[i].airplane_B.airplane_Name, B.airplane_Name);
    int insertByPlaneBName = equalDistance && planeANameComp == 0 && planeBNameComp > 0;
    //// equal distance and plane names => insert by plane A coordinate X
    int planeACoordXComp = airplanePairDatabase[i].airplane_A.coordinates.x > A.coordinates.x;
    int insertByPlaneACoordX = equalDistance && planeANameComp == 0 && planeBNameComp == 0 && planeACoordXComp;
    int equalPlaneACoordX = equalDistance && planeANameComp == 0 && planeBNameComp == 0 && airplanePairDatabase[i].airplane_A.coordinates.x == A.coordinates.x;
    //// equal distance, plane names and plane A coordinate X => insert by plane A coordinate Y
    int planeACoordYComp = airplanePairDatabase[i].airplane_A.coordinates.y > A.coordinates.y;
    int insertByPlaneACoordY = equalPlaneACoordX && planeACoordYComp;
    int equalPlaneACoordY = equalPlaneACoordX && airplanePairDatabase[i].airplane_A.coordinates.y > A.coordinates.y;
    //// equal distance, plane names and both plane A coordinates => insert by plane B coordinate X
    int planeBCoordXComp = airplanePairDatabase[i].airplane_B.coordinates.x > B.coordinates.x;
    int insertByPlaneBCoordX = equalPlaneACoordY && planeBCoordXComp;
    int equalPlaneBCoordX = equalPlaneACoordY && airplanePairDatabase[i].airplane_A.coordinates.y == A.coordinates.y;
    //// equal distance, plane names, both plane A coordinates and plane B coordinate x => insert by plane B coordinate Y
    int planeBCoordYComp = airplanePairDatabase[i].airplane_B.coordinates.y > B.coordinates.y;
    int insertByPlaneBCoordY = equalPlaneBCoordX && planeBCoordYComp;
    int insertExpr = insertByDistance || insertByPlaneAName || insertByPlaneBName || insertByPlaneACoordX || insertByPlaneACoordY || insertByPlaneBCoordX || insertByPlaneBCoordY;
    return insertExpr;
}
/**
 * Function which inserts the airplane pair in the right place in the database.
 * Primarily sorted by distance, secondly by the name of airplane A and lastly by name of airplane B.
 * @param[in] airplanePairDatabase Pointer to database of pairs.
 * @param[in] numberOfPairs Number of pairs.
 * @param[in] distance Distance between airplanes.
 * @param[in] airplane_A Airplane A.
 * @param[in] airplane_B Airplane B.
 */
void insertAirplanePair(s_airplanePair * airplanePairDatabase, int *numberOfPairs, double distance, s_airplane * airplane_A, s_airplane * airplane_B){
    if (strcmp(airplane_A->airplane_Name,airplane_B->airplane_Name) > 0){
        s_airplane * tmp = airplane_A;
        airplane_A = airplane_B;
        airplane_B = tmp;
    }
    if ( ! binarySearchAirplanePair(airplanePairDatabase, *numberOfPairs, distance, *airplane_A, *airplane_B) ){
        int i;
        for (i = (*numberOfPairs) - 1; insertAirplanePairExprEval(airplanePairDatabase, i, distance, *airplane_A, *airplane_B); i--){
            airplanePairDatabase[i+1] = airplanePairDatabase[i];
        }
        airplanePairDatabase[i+1].distance = distance;
        airplanePairDatabase[i+1].airplane_A = *airplane_A;
        airplanePairDatabase[i+1].airplane_B = *airplane_B;
        (*numberOfPairs)++;
    }
}
/**
 * Function which uses brute force approach to finding the smallest distance.
 * @param[in] airplaneDatabase Pointer to airplanes database.
 * @param[in] numberOfPlanes Number of airplanes.
 * @param[in,out] outputDatabasePtr Pointer to output database.
 * @param[in,out] numberOfPairsPtr Number of pairs in output database.
 * @return Smallest distance found.
 */
double bruteForceMinDistance(s_airplane * airplaneDatabase, int numberOfPlanes, s_airplanePair * outputDatabasePtr, int * numberOfPairsPtr){
    int numberOfPairs = *numberOfPairsPtr;
    double minDistance = DBL_MAX;
    for (int i = 0; i < numberOfPlanes; ++i){
        for (int j = i+1; j < numberOfPlanes; ++j){
            double planesDistance = planeDist(airplaneDatabase[i], airplaneDatabase[j]);
            if (planesDistance < minDistance || dbl_comp(planesDistance, minDistance)){
                minDistance=planeDist(airplaneDatabase[i], airplaneDatabase[j]);
                insertAirplanePair(outputDatabasePtr, &numberOfPairs, minDistance, &airplaneDatabase[i], &airplaneDatabase[j]);
            }
        }
    }
    *numberOfPairsPtr = numberOfPairs;
    return minDistance;
}
/**
 * Function which finds the smallest distance between points inside a strip.
 * Uses brute force which has a time complexity of (n^2). But there is proof that the number of comparisons is constant.
 * @see https://en.wikipedia.org/wiki/Closest_pair_of_points_problem
 * The strip holds the airplanes (points) which might have been split in the beginning of the recursive splitting.
 * @param[in] strip Array of airplanes (points) which are in the middle strip of the 2d space.
 * @param[in] numberOfAirplanesInStrip Number of airplanes inside the strip
 * @param[in] distance Max distance found so far.
 * @param[in,out] outputDatabasePtr Pointer to output database containing pairs.
 * @param[in,out] numberOfPairsPtr Number of pairs in the database.
 * @return Smallest distance between points in the strip.
 */
double stripMinDistance(s_airplane *strip, int numberOfAirplanesInStrip, double distance, s_airplanePair * outputDatabasePtr, int * numberOfPairsPtr){
    int numberOfPairs = *numberOfPairsPtr;
    double minDistance = distance;
    qsort(strip, numberOfAirplanesInStrip, sizeof(s_airplane), airplaneCompareY);
    for (int i = 0; i < numberOfAirplanesInStrip; ++i){
        for (int j = i+1;
             j < numberOfAirplanesInStrip && (strip[j].coordinates.y - strip[i].coordinates.y < minDistance || dbl_comp(strip[j].coordinates.y - strip[i].coordinates.y, minDistance));
            ++j){
            double planeDistance = planeDist(strip[i],strip[j]);
            if ( planeDistance < minDistance || dbl_comp(planeDistance,minDistance) ){
                minDistance = planeDist(strip[i], strip[j]);
                insertAirplanePair(outputDatabasePtr, &numberOfPairs, minDistance, &strip[i], &strip[j]);
            }
        }
    }
    *numberOfPairsPtr = numberOfPairs;
    return minDistance;
}
/**
 * Calculates the closest pair of airplanes using the divide and conquer algorithm.
 * This requires the planes to be sorted by the X coordinate.
 * More information about the algorithm
 * @see https://en.wikipedia.org/wiki/Closest_pair_of_points_problem
 * @param[in] airplaneDatabase Pointer to database of airplanes
 * @param[in] numberOfPlanes Number of saved airplanes
 * @return Distance of closest airplanes
 */
double closestAirplanesRec(s_airplane * airplaneDatabase, int numberOfPlanes, s_airplanePair * outputDatabasePtr, int * numberOfPairsPtr){
    s_airplanePair * outputDatabase = outputDatabasePtr;
    int numberOfPairs = * numberOfPairsPtr;
    //// If the amount of Airplanes is small (<= 3), use brute force.
    if(numberOfPlanes <= 3){
        double bruteForceDistance = bruteForceMinDistance(airplaneDatabase, numberOfPlanes, outputDatabase, &numberOfPairs);
        *numberOfPairsPtr = numberOfPairs;
        *outputDatabasePtr = *outputDatabase;
        return bruteForceDistance;
    }
    int middleIndex = numberOfPlanes / 2;
    s_airplane midAirplane = airplaneDatabase[middleIndex];
    //// Recursively find the smallest distance in the left a right sides of the plane.
    double dl = closestAirplanesRec(airplaneDatabase, middleIndex, outputDatabase, &numberOfPairs);
    double dr = closestAirplanesRec(airplaneDatabase + middleIndex, numberOfPlanes - middleIndex, outputDatabase,&numberOfPairs);
    double d = dblMin(dl, dr);
    //// Build an array of airplanes which have the maximum distance of d from the middle line.
    //// This forms a strip in the 2D space. All points inside this strip must be distance checked because they were split
    //// in the previous recursive step of the algorithm.
    s_airplane * strip = (s_airplane*)malloc(numberOfPlanes * sizeof(*strip));
    int j = 0;
    for (int i = 0; i < numberOfPlanes; i++) {
        double xDistance = fabs(airplaneDatabase[i].coordinates.x - midAirplane.coordinates.x);
        if (xDistance < d || dbl_comp(xDistance,d)) {
            strip[j] = airplaneDatabase[i];
            j++;
        }
    }
    //// Find the distance of the closest points in the strip. Return the smaller distance.
    double stripMinDist = stripMinDistance(strip, j, d, outputDatabase, &numberOfPairs);
    *numberOfPairsPtr = numberOfPairs;
    *outputDatabasePtr = *outputDatabase;
    free(strip);
    return dblMin(d,stripMinDist);
}
/**
 * Fucntion which allocates memory for the output pairs database and
 * enters the recursive Closest Points Divide and Conquer algorithm.
 * @param[in] airplaneDatabase Pointer to airplane database
 * @param[in] numberOfPlanes Number of airplanes
 * @param[out] outputDatabasePtr Pointer to database of airplane pairs.
 * @param[out] numberOfPairsPtr Number of pairs with same distance from each other
 * @return Returns minimal distance between two airplanes.
 */
double closestAirplanes(s_airplane * airplaneDatabase, int numberOfPlanes, s_airplanePair ** outputDatabasePtr, int * numberOfPairsPtr){
    s_airplanePair * outputDatabase = (s_airplanePair *)malloc(numberOfPlanes * numberOfPlanes * sizeof(*outputDatabase));
    double distance = closestAirplanesRec(airplaneDatabase, numberOfPlanes, outputDatabase, numberOfPairsPtr);
    *outputDatabasePtr = outputDatabase;
    return distance;
}
/**
 * Main function.
 * Program works in steps.
 * 1) Read the input and save all the airplanes in a sorted way.
 * 2) Use the Closest Pair Of Points algorithm to find all the pairs of planes with the minimal distance found between them.
 * 3) Print the pairs to the standard output.
 * Output handling:
 * Create an output database of airplane pairs. Which will be sorted by distance.
 * This database will have a maximum size of n*n (n being the number of planes) and will not be filled fully!
 * Because the number of comparisons in between points is lowered by the closest points - divide and conquer algorithm
 * @return 0 if everything went fine, otherwise returns 1.
 */
int main(void) {
    int numberOfPlanes = 0;
    s_airplane * airplaneDatabase = NULL;
    printf("Zadejte lety:\n");
    if (readInput(&airplaneDatabase, &numberOfPlanes)){
        printf("Nespravny vstup.\n");
        return 1;
    }
    int numberOfPairs = 0;
    s_airplanePair * outputDatabase = NULL;
    printf("Nejmensi vzdalenost: %lf\n",closestAirplanes(airplaneDatabase, numberOfPlanes, &outputDatabase, &numberOfPairs));
    printPairs(outputDatabase,numberOfPairs);
    free(outputDatabase);
    freePlanes(airplaneDatabase,numberOfPlanes);
    return 0;
}


