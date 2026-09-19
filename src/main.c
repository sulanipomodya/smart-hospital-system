#include <stdio.h>
#include "hospital.h"

/* Calculates and prints the bill for patient i right after they're registered,
   using the Requirement 3 formulas, then saves the results back into
   patientFinalBill/patientDiscount (needed by the Requirement 6 report) and
   logs the patient to file (Requirement 7). */
static void generateBillForPatient(int i) {
    int specIdx = patientSpecialtyID[i] - 1;
    double baseFee = specialtyFees[specIdx];

    double waitTime  = calcWaitTime(specIdx);
    double surcharge = calcSurcharge(patientUrgency[i], baseFee);

    int wardIdx = (patientAdmitted[i] == 1) ? patientWardID[i] - 1 : 0;
    double wardCost = calcWardCost(patientDaysAdmitted[i], wardIdx);

    double gross    = calcGrossTotal(baseFee, surcharge, wardCost);
    double discount = calcDiscount(patientAge[i], gross);
    double finalAmt = calcFinalPayable(gross, discount);

    patientFinalBill[i] = finalAmt;
    patientDiscount[i]  = discount;

    printBill(i, waitTime, surcharge, wardCost, gross, discount, finalAmt);
    appendPatientRecord(i, finalAmt);
}

/* Requirement 4's sort, displayed as a readable list */
static void printPriorityList(void) {
    if (patientCount == 0) {
        printf("\nNo patients registered yet.\n");
        return;
    }
    int order[MAX_PATIENTS];
    sortByPriority(order);

    printf("\n--- Patients in Priority Order ---\n");
    for (int i = 0; i < patientCount; i++) {
        int idx = order[i];
        printf("%d. PAT-%04d | %-20s | Level %d | %s\n",
               i + 1, 1000 + idx + 1, patientName[idx], patientUrgency[idx],
               specialtyNames[patientSpecialtyID[idx] - 1]);
    }
}

/* Discards leftover characters after scanf("%d", ...) so the next menu read
   doesn't get confused by a leftover newline. Same trick as in patient.c. */
static void clearBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { /* discard */ }
}

int main(void) {
    initBedOccupancy();
    loadBedStatus(); /* pick up bed state saved from a previous run, if any */

    int choice;
    do {
        printf("\n==================================\n");
        printf(" SMART HOSPITAL MENU\n");
        printf("==================================\n");
        printf("1. Register New Patient (auto-generates bill)\n");
        printf("2. View Bed Occupancy Status\n");
        printf("3. View Patients in Priority Order\n");
        printf("4. View Summary Report\n");
        printf("5. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            clearBuffer();
            choice = -1; /* force "invalid choice" below instead of crashing on bad input */
        } else {
            clearBuffer();
        }

        switch (choice) {
            case 1:
                registerPatient();
                generateBillForPatient(patientCount - 1);
                break;
            case 2:
                printBedStatus();
                break;
            case 3:
                printPriorityList();
                break;
            case 4:
                printSummaryReport();
                break;
            case 5:
                printf("\nSaving bed status and exiting. Goodbye!\n");
                saveBedStatus();
                break;
            default:
                printf("\nInvalid choice, please enter a number from 1 to 5.\n");
        }
    } while (choice != 5);

    return 0;
}
