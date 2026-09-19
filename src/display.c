#include <stdio.h>
#include <string.h>
#include "hospital.h"

/* Formats a number like 56750.00 into "56,750.00" (comma every 3 digits).
   Handles negative numbers too (used for the discount line, shown as -8,512.50). */
static void formatCurrency(double amount, char *out) {
    char sign[2] = "";
    if (amount < 0) {
        strcpy(sign, "-");
        amount = -amount;
    }

    char plain[32];
    sprintf(plain, "%.2f", amount); /* e.g. "56750.00" */

    char *dot = strchr(plain, '.');
    int intLen = (int)(dot - plain); /* number of digits before the decimal point */

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

    sprintf(out, "%s%s%s", sign, withCommas, dot); /* sign + int-with-commas + ".xx" */
}

static const char *urgencyLabel(int level) {
    if (level == 3) return "Critical";
    if (level == 2) return "Urgent";
    return "Normal";
}

static int surchargePercent(int level) {
    if (level == 3) return 50;
    if (level == 2) return 20;
    return 0;
}

void printBill(int i, double waitTime, double surcharge, double wardCost,
               double grossTotal, double discount, double finalPayable) {
    char buf[32];
    int specIdx = patientSpecialtyID[i] - 1;
    double baseFee = specialtyFees[specIdx];

    printf("\n====================================================\n");
    printf(" SMART HOSPITAL ADMISSION & BILL\n");
    printf("----------------------------------------------------------------------------------------\n");
    printf("Patient ID          : PAT-%04d\n", 1000 + i + 1);
    printf("Patient Name         : %s\n", patientName[i]);

    printf("Age                  : %d Years", patientAge[i]);
    if (patientAge[i] < 5 || patientAge[i] > 65) {
        printf(" (15%% Subsidy Eligible)");
    }
    printf("\n");

    printf("Specialty            : %s\n", specialtyNames[specIdx]);

    if (patientAdmitted[i] == 1) {
        printf("Assigned Ward        : %s (Bed #%02d)\n",
               wardNames[patientWardID[i] - 1], patientBedIndex[i] + 1);
    } else {
        printf("Assigned Ward        : Not Admitted (Outpatient)\n");
    }

    printf("Urgency Level        : Level %d (%s)\n", patientUrgency[i], urgencyLabel(patientUrgency[i]));
    printf("----------------------------------------------------------------------------------------\n");

    formatCurrency(baseFee, buf);
    printf("Base Consultation Fee    : LKR %s\n", buf);

    formatCurrency(surcharge, buf);
    printf("Emergency Surcharge      : LKR %s (%d%%)\n", buf, surchargePercent(patientUrgency[i]));

    if (patientAdmitted[i] == 1) {
        formatCurrency(wardCost, buf);
        printf("Ward Stay Cost (%d Days) : LKR %s\n", patientDaysAdmitted[i], buf);
    } else {
        printf("Ward Stay Cost           : LKR 0.00\n");
    }
    printf("----------------------------------------------------------------------------------------\n");

    formatCurrency(grossTotal, buf);
    printf("Gross Total Bill         : LKR %s\n", buf);

    formatCurrency(-discount, buf); /* shown as a negative line, matching the spec sample */
    printf("Age Subsidy Discount     : LKR %s (%s)\n", buf,
           (patientAge[i] < 5 || patientAge[i] > 65) ? "15%" : "0%");
    printf("----------------------------------------------------------------------------------------\n");

    formatCurrency(finalPayable, buf);
    printf("Final Payable Amount     : LKR %s\n", buf); /* Shows the final amount which has to pay */

    if (patientUrgency[i] == 3) {
        printf("Estimated Waiting Time   : 0.00 mins (Immediate Attention)\n");
    } else {
        printf("Estimated Waiting Time   : %.2f mins\n", waitTime);
    }
    printf("====================================================\n");
}
