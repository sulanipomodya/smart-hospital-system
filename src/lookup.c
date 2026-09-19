#include <stdio.h>
#include "hospital.h"

/* Doctor specialty lookup data */
const char specialtyNames[NUM_SPECIALTIES][30] = {
    "General Practice (OPD)", "Paediatrics", "Cardiology", "Neurology"
};
const double specialtyFees[NUM_SPECIALTIES]            = {1500.00, 2500.00, 4500.00, 5000.00};
const int    specialtyTimePerPatient[NUM_SPECIALTIES]  = {15, 20, 30, 30};
const int    specialtyDailyCap[NUM_SPECIALTIES]        = {30, 20, 12, 10};
int          specialtyQueueCount[NUM_SPECIALTIES]      = {0, 0, 0, 0};

/* Hospital ward lookup data */
const char wardNames[NUM_WARDS][30] = {
    "General Ward", "Paediatric Ward", "Surgical Ward", "ICU (Intensive Care Unit)"
};
const double wardDailyRate[NUM_WARDS]   = {3000.00, 6000.00, 12000.00, 25000.00};
const int    wardBedCapacity[NUM_WARDS] = {20, 10, 10, 5};

/* Bed status matrix: rows = wards, columns = bed slots within that ward */
int bedOccupancy[NUM_WARDS][MAX_BEDS_PER_WARD];

void initBedOccupancy(void) {
    for (int w = 0; w < NUM_WARDS; w++) {
        for (int b = 0; b < MAX_BEDS_PER_WARD; b++) {
            bedOccupancy[w][b] = 0; /* all beds start free */
        }
    }
}

void printBedStatus(void) {
    printf("\n---------------------------------------------------\n");
    printf(" BED OCCUPANCY STATUS\n");
    printf("---------------------------------------------------\n");
    for (int w = 0; w < NUM_WARDS; w++) {
        int occupied = 0;
        for (int b = 0; b < wardBedCapacity[w]; b++) {
            if (bedOccupancy[w][b] == 1) occupied++;
        }
        printf("%-28s : %2d / %2d beds occupied\n", wardNames[w], occupied, wardBedCapacity[w]);
    }
    printf("---------------------------------------------------\n");
}

/* Finds the first free bed slot in a ward. Returns -1 if the ward is full. */
int findFreeBed(int wardIndex) {
    for (int b = 0; b < wardBedCapacity[wardIndex]; b++) {
        if (bedOccupancy[wardIndex][b] == 0) {
            return b;
        }
    }
    return -1;
}
