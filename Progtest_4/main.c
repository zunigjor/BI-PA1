//// INCLUDE
#include <stdio.h>
#include <stdlib.h>
//// DEFINE
#define MAX_SIZE 200000
//// SPECIAL DATATYPE
/*
 * Using struct to make navigating variables easier.
 * Array could be used because all data types are ints.
 */
typedef struct waterTank{
    int floorAltitude;
    int height;
    int width;
    int depth;
    int maxCapacity;
    int totalHeight;
} SwaterTank;
//// PROGRAM FUNCTION DECLARATIONS
/*
 * Read input of water tanks.
 * return
 * 0 := all went well
 * 1 := wrong input
 */
int readInput(SwaterTank*, int*,int*,int*);
/*
 * Comparator func to sort water tanks
 */
int compWaterTank(const void *, const void *);
/*
 *  Prints the water level height
 */
void printWaterLevel(SwaterTank*, int, int, int, int);
/*
 * Calculates the water level
 */
double calculateWaterLevel(SwaterTank*, int, int, int);
/*todo delete this later*/
void printDatabase(SwaterTank *,int);
//// MAIN
int main(void) {
    SwaterTank waterTanksArr[MAX_SIZE];
    int numberOfTanks;
    int maxTanksCapacitySum;
    int highestPoint;
    if (readInput(waterTanksArr,&numberOfTanks,&maxTanksCapacitySum,&highestPoint)){
        printf("Nespravny vstup.\n");
        return 1;
    }
    //printDatabase(waterTanksArr,numberOfTanks);
    qsort(waterTanksArr,numberOfTanks, sizeof(SwaterTank),compWaterTank);
    //printDatabase(waterTanksArr,numberOfTanks);
    printf("Zadejte objem vody:\n");
    int scanCode, volumeInput;
    while ((scanCode = scanf("%d",&volumeInput)) != EOF){
        if(scanCode != 1 || volumeInput < 0){
            printf("Nespravny vstup.\n");
            return 1;
        }
        printWaterLevel(waterTanksArr, numberOfTanks, maxTanksCapacitySum,highestPoint, volumeInput);
    }
    return 0;
}
//// PROGRAM FUNCTION DEFINITIONS
int readInput(SwaterTank * waterTanksArr, int * numOfTanksOutput,int * maxCapacitySumOutput,int * highestPointOutput){
    int numOfTanks;
    // Manage number of tanks
    printf("Zadejte pocet nadrzi:\n");
    if (scanf("%d",&numOfTanks) != 1 || numOfTanks < 1 || numOfTanks > 200000){
        return 1;
    }
    *numOfTanksOutput = numOfTanks;
    // save tanks into array
    int maxCapacitySum=0;
    int highestPoint=0;
    printf("Zadejte parametry nadrzi:\n");
    int alt, hei, wid, dep;
    for (int i = 0; i < numOfTanks; ++i){
        if(scanf("%d %d %d %d", &alt, &hei, &wid, &dep) != 4){
            return 1;
        }
        if (hei < 1 || wid < 1 || dep < 1){
            return 1;
        }
        waterTanksArr[i].floorAltitude = alt;
        waterTanksArr[i].height  = hei;
        waterTanksArr[i].width  = wid;
        waterTanksArr[i].depth = dep;
        waterTanksArr[i].maxCapacity = hei * wid * dep;
        waterTanksArr[i].totalHeight = alt + hei;
        maxCapacitySum += hei * wid * dep;
        if (i == 0){
            highestPoint = waterTanksArr[i].totalHeight;
        } else {
            if (highestPoint < waterTanksArr[i].totalHeight){
                highestPoint = waterTanksArr[i].totalHeight;
            }
        }
    }
    *highestPointOutput = highestPoint;
    *maxCapacitySumOutput = maxCapacitySum;
    return 0;
}
int compWaterTank(const void * a, const void * b){
    const SwaterTank * ptr1 = (SwaterTank *)a;
    const SwaterTank * ptr2 = (SwaterTank *)b;
    if (ptr1->floorAltitude < ptr2->floorAltitude){
        return -1;
    } else if (ptr1->floorAltitude > ptr2->floorAltitude) {
        return 1;
    } else if (ptr1->height < ptr2 ->height) {
        return -1;
    } else if (ptr1->height > ptr2->height){
        return 1;
    } else {
        return 0;
    }
}
void printWaterLevel(SwaterTank * data, int numOfTanks, int maxCapSum,int highestPoint, int inputVol){
    double waterlevel;
    if (inputVol == 0){
        printf("Prazdne.\n");
    } else if (inputVol > maxCapSum){
        printf("Pretece.\n");
    } else {
        waterlevel = calculateWaterLevel(data,numOfTanks,inputVol,highestPoint);
        printf("h = %lf\n",waterlevel);
    }
}
double calculateWaterLevel(SwaterTank* tanksArr, int numOfTanks, int volume,int highestPoint){
    int min = tanksArr[0].floorAltitude;
    int max = highestPoint;
    int size = max - min;
    int * meters = (int*)calloc(size, sizeof(int));
    int numberOfNegativeIntervals = -min;
    for (int i = 0; i < numOfTanks; i++) {
        for (int j = tanksArr[i].floorAltitude + numberOfNegativeIntervals;
                 j < tanksArr[i].floorAltitude + tanksArr[i].height + numberOfNegativeIntervals; j++){
            meters[j] += tanksArr[i].width * tanksArr[i].depth;
        }
    }
    double waterLevel=0;
    int filledIntervals=0;
    for (int k = 0; k < size; ++k) {
        if (volume > meters[k]){
            volume -= meters[k];
            filledIntervals++;
        } else {
            waterLevel = (double)volume / (double)meters[k];
            waterLevel += filledIntervals;
            waterLevel -= numberOfNegativeIntervals;
            break;
        }
    }
    free(meters);
    return waterLevel;
}
////PRINT FUCTION
void printDatabase(SwaterTank * data,int len){
    printf("Tanks : Alt / h / w / d \n");
    for (int i = 0; i < len; ++i) {
        printf("Tank %d:   %d / %d / %d / %d \n",i,data[i].floorAltitude,data[i].height,data[i].width,data[i].depth);
    }
}