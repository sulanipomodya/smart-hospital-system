#include <stdio.h>
#include <string.h>
#include "hospital.h"

/* Parallel arrays: index i holds all of patient i's data across every array */
char   patientName[MAX_PATIENTS][NAME_LEN];
int    patientAge[MAX_PATIENTS];
int    patientUrgency[MAX_PATIENTS];
int    patientSpecialtyID[MAX_PATIENTS];
int    patientAdmitted[MAX_PATIENTS];
int    patientWardID[MAX_PATIENTS];
int    patientBedIndex[MAX_PATIENTS];
int    patientDaysAdmitted[MAX_PATIENTS];
double patientFinalBill[MAX_PATIENTS];
double patientDiscount[MAX_PATIENTS];
int    patientCount = 0;

/* Clears leftover characters in the input buffer after a scanf().
   Needed before fgets(), otherwise fgets reads a leftover '\n' and returns an empty line. */
static void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { /* discard */ }
}

void registerPatient(void) {
    if (patientCount >= MAX_PATIENTS) {
        printf("\nPatient list is full. Cannot register more patients.\n");
        return;
    }

    int i = patientCount; /* index this new patient will occupy */

    printf("\n----- New Patient Registration -----\n");

    /* --- Name --- */
    printf("Patient Name: ");
    fgets(patientName[i], NAME_LEN, stdin);
    patientName[i][strcspn(patientName[i], "\n")] = '\0'; /* strip trailing newline */

    /* --- Age --- */
    do {
        printf("Patient Age: ");
        scanf("%d", &patientAge[i]);
        clearInputBuffer();
        if (patientAge[i] < 0 || patientAge[i] > 120) {
            printf("Please enter a realistic age (0-120).\n");
        }
    } while (patientAge[i] < 0 || patientAge[i] > 120);

    /* --- Urgency level --- */
    do {
        printf("Urgency Level (1 = Normal, 2 = Urgent, 3 = Critical): ");
        scanf("%d", &patientUrgency[i]);
        clearInputBuffer();
        if (patientUrgency[i] < 1 || patientUrgency[i] > 3) {
            printf("Please enter 1, 2, or 3.\n");
        }
    } while (patientUrgency[i] < 1 || patientUrgency[i] > 3);

    /* --- Specialty selection --- */
    printf("\nAvailable Specialties:\n");
    for (int s = 0; s < NUM_SPECIALTIES; s++) {
        printf("  %d. %s (LKR %.2f)\n", s + 1, specialtyNames[s], specialtyFees[s]);
    }
    do {
        printf("Select Specialty ID (1-%d): ", NUM_SPECIALTIES);
        scanf("%d", &patientSpecialtyID[i]);
        clearInputBuffer();
        if (patientSpecialtyID[i] < 1 || patientSpecialtyID[i] > NUM_SPECIALTIES) {
            printf("Please enter a number between 1 and %d.\n", NUM_SPECIALTIES);
        }
    } while (patientSpecialtyID[i] < 1 || patientSpecialtyID[i] > NUM_SPECIALTIES);

    /* Increment that specialty's queue count now that this patient has joined it */
    specialtyQueueCount[patientSpecialtyID[i] - 1]++;

    /* --- Ward admission --- */
    do {
        printf("\nIs Patient Admitted to a Ward? (1 = Yes, 0 = No): ");
        scanf("%d", &patientAdmitted[i]);
        clearInputBuffer();
        if (patientAdmitted[i] != 0 && patientAdmitted[i] != 1) {
            printf("Please enter 1 or 0.\n");
        }
    } while (patientAdmitted[i] != 0 && patientAdmitted[i] != 1);

    if (patientAdmitted[i] == 1) {
        int wardID, bedIndex;
        do {
            printf("\nAvailable Wards:\n");
            for (int w = 0; w < NUM_WARDS; w++) {
                printf("  %d. %s (LKR %.2f/day)\n", w + 1, wardNames[w], wardDailyRate[w]);
            }
            printf("Select Ward ID (1-%d): ", NUM_WARDS);
            scanf("%d", &wardID);
            clearInputBuffer();
            if (wardID < 1 || wardID > NUM_WARDS) {
                printf("Please enter a number between 1 and %d.\n", NUM_WARDS);
                continue;
            }
            bedIndex = findFreeBed(wardID - 1);
            if (bedIndex == -1) {
                printf("%s is full. Please choose a different ward.\n", wardNames[wardID - 1]);
            }
        } while (wardID < 1 || wardID > NUM_WARDS || bedIndex == -1);

        patientWardID[i]   = wardID;
        patientBedIndex[i] = bedIndex;
        bedOccupancy[wardID - 1][bedIndex] = 1; /* mark bed occupied */

        do {
            printf("Days Admitted: ");
            scanf("%d", &patientDaysAdmitted[i]);
            clearInputBuffer();
            if (patientDaysAdmitted[i] <= 0) {
                printf("Please enter a positive number of days.\n");
            }
        } while (patientDaysAdmitted[i] <= 0);

        printf("Assigned to %s, Bed #%02d\n", wardNames[wardID - 1], bedIndex + 1);
    } else {
        patientWardID[i]       = 0;
        patientBedIndex[i]     = -1;
        patientDaysAdmitted[i] = 0;
    }

    patientFinalBill[i] = 0.0; /* calculated later in billing stage */
    patientCount++;

    printf("\nPatient registered successfully. Patient ID: PAT-%04d\n", 1000 + i + 1);
}
