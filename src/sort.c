#include "hospital.h"

/* Fills order[] with patient indices (0 .. patientCount-1) sorted so that:
   - Primary: higher urgency first (3 Critical, then 2 Urgent, then 1 Normal)
   - Secondary: for equal urgency, earlier-registered patients stay first

   Uses bubble sort. It only swaps when the LEFT patient's urgency is strictly
   LESS than the right patient's urgency - never when they're equal - which is
   what keeps the sort stable (equal-urgency patients keep their original
   registration order instead of getting shuffled). */
void sortByPriority(int order[MAX_PATIENTS]) {
    for (int i = 0; i < patientCount; i++) {
        order[i] = i; /* start in registration order */
    }

    for (int i = 0; i < patientCount - 1; i++) {
        for (int j = 0; j < patientCount - 1 - i; j++) {
            if (patientUrgency[order[j]] < patientUrgency[order[j + 1]]) {
                int temp     = order[j];
                order[j]     = order[j + 1];
                order[j + 1] = temp;
            }
        }
    }
}
