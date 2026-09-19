#include <stdio.h>
#include "hospital.h"

/* Writes the current bed occupancy matrix to beds_status.txt, one line per ward,
   space-separated 0s and 1s. Called when the program exits so the next run can
   pick up where this one left off. */
void saveBedStatus(void) {
    FILE *fp = fopen("beds_status.txt", "w");
    if (fp == NULL) {
        printf("Warning: could not save bed status to file.\n");
        return;
    }
    for (int w = 0; w < NUM_WARDS; w++) {
        for (int b = 0; b < MAX_BEDS_PER_WARD; b++) {
            fprintf(fp, "%d ", bedOccupancy[w][b]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

/* Reads beds_status.txt back into the bedOccupancy matrix at program startup.
   If the file doesn't exist yet (first ever run), leaves the matrix as all-free,
   which initBedOccupancy() already set up - so this is safe to call every time. */
void loadBedStatus(void) {
    FILE *fp = fopen("beds_status.txt", "r");
    if (fp == NULL) {
        return; /* no saved file yet - not an error, just a first run */
    }
    for (int w = 0; w < NUM_WARDS; w++) {
        for (int b = 0; b < MAX_BEDS_PER_WARD; b++) {
            if (fscanf(fp, "%d", &bedOccupancy[w][b]) != 1) {
                fclose(fp);
                return; /* file was shorter/corrupted - stop reading safely */
            }
        }
    }
    fclose(fp);
}

/* Appends one line per patient to patient_records.txt (never overwritten, so it
   builds up a permanent log across every run of the program). */
void appendPatientRecord(int i, double finalPayable) {
    FILE *fp = fopen("patient_records.txt", "a");
    if (fp == NULL) {
        printf("Warning: could not write to patient records file.\n");
        return;
    }
    fprintf(fp, "PAT-%04d | %s | Age %d | Urgency %d | Specialty %s | Final Bill LKR %.2f\n",
            1000 + i + 1, patientName[i], patientAge[i], patientUrgency[i],
            specialtyNames[patientSpecialtyID[i] - 1], finalPayable);
    fclose(fp);
}
