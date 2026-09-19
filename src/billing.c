#include "hospital.h"

/* Wait Time = Current Queue Count for Selected Specialty x Avg Time per Patient.
   specialtyQueueCount is incremented in registerPatient() as soon as the specialty
   is chosen, so by the time this is called the count already includes the patient
   themselves - matching "queue count after registration" from the spec note. */
double calcWaitTime(int specialtyIndex) {
    return specialtyQueueCount[specialtyIndex] * specialtyTimePerPatient[specialtyIndex];
}

/* Emergency Surcharge: 0% / 20% / 50% of base fee depending on urgency level */
double calcSurcharge(int urgencyLevel, double baseFee) {
    if (urgencyLevel == 2) {
        return baseFee * 0.20;
    } else if (urgencyLevel == 3) {
        return baseFee * 0.50;
    }
    return 0.0; /* urgencyLevel == 1 */
}

/* Total Ward Stay Cost = Days Admitted x Ward Daily Rate (0 if not admitted) */
double calcWardCost(int daysAdmitted, int wardIndex) {
    if (daysAdmitted <= 0) {
        return 0.0;
    }
    return daysAdmitted * wardDailyRate[wardIndex];
}

/* Gross Total = Base Fee + Surcharge + Ward Cost */
double calcGrossTotal(double baseFee, double surcharge, double wardCost) {
    return baseFee + surcharge + wardCost;
}

/* Age Subsidy Discount: 15% of gross total if age < 5 or age > 65, else 0 */
double calcDiscount(int age, double grossTotal) {
    if (age < 5 || age > 65) {
        return grossTotal * 0.15;
    }
    return 0.0;
}

/* Final Payable Amount = Gross Total - Discount */
double calcFinalPayable(double grossTotal, double discount) {
    return grossTotal - discount;
}
