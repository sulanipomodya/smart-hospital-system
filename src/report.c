#include <stdio.h>
#include <string.h>
#include "hospital.h"

/* Same comma-formatting helper as display.c (kept local to this file to avoid
   pulling display.c in as a dependency just for one small function). */
static void formatCurrency(double amount, char *out) {
    char sign[2] = "";
    if (amount < 0) {
        strcpy(sign, "-");
        amount = -amount;
    }
    char plain[32];
    sprintf(plain, "%.2f", amount);
    char *dot = strchr(plain, '.');
    int intLen = (int)(dot - plain);
    char withCommas[32];
    int wi = 0;
    for (int i = 0; i < intLen; i++) {
        withCommas[wi++] = plain[i];
        int digitsLeft = intLen - i - 1;
        if (digitsLeft > 0 && digitsLeft % 3 == 0) {
            withCommas[wi++] = ',';
        }
    }
    withCommas[wi] = '\0';
    sprintf(out, "%s%s%s", sign, withCommas, dot);
}

void printSummaryReport(void) {
    if (patientCount == 0) {
        printf("\nNo patients registered yet.\n");
        return;
    }

    int normalCount = 0, urgentCount = 0, criticalCount = 0;
    double totalRevenue = 0.0, totalDiscount = 0.0;
    int highestIndex = 0;

    for (int i = 0; i < patientCount; i++) {
        if (patientUrgency[i] == 1) normalCount++;
        else if (patientUrgency[i] == 2) urgentCount++;
        else criticalCount++;

        totalRevenue  += patientFinalBill[i];
        totalDiscount += patientDiscount[i];

        if (patientFinalBill[i] > patientFinalBill[highestIndex]) {
            highestIndex = i;
        }
    }

    char buf[32];

    printf("\n============================================\n");
    printf(" HOSPITAL SUMMARY REPORT\n");
    printf("============================================\n");
    printf("Total Patients Registered : %d\n", patientCount);
    printf("  Level 1 (Normal)         : %d\n", normalCount);
    printf("  Level 2 (Urgent)         : %d\n", urgentCount);
    printf("  Level 3 (Critical)       : %d\n", criticalCount);
    printf("--------------------------------------------\n");

    formatCurrency(totalRevenue, buf);
    printf("Total Revenue Earned      : LKR %s\n", buf);
    formatCurrency(totalDiscount, buf);
    printf("Total Discounts Granted   : LKR %s\n", buf);
    printf("--------------------------------------------\n");

    for (int w = 0; w < NUM_WARDS; w++) {
        int occupied = 0;
        for (int b = 0; b < wardBedCapacity[w]; b++) {
            if (bedOccupancy[w][b] == 1) occupied++;
        }
        double pct = (occupied * 100.0) / wardBedCapacity[w];
        printf("%-28s : %.1f%% occupied (%d/%d)\n", wardNames[w], pct, occupied, wardBedCapacity[w]);
    }
    printf("--------------------------------------------\n");

    formatCurrency(patientFinalBill[highestIndex], buf);
    printf("Highest-Paying Patient    : %s (LKR %s)\n", patientName[highestIndex], buf);
    printf("============================================\n");
}
