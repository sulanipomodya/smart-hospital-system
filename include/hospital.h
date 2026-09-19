#ifndef HOSPITAL_H
#define HOSPITAL_H

/* ---------- Constants ---------- */
#define MAX_PATIENTS     100
#define NUM_SPECIALTIES  4
#define NUM_WARDS        4
#define MAX_BEDS_PER_WARD 20
#define NAME_LEN         50

/* ---------- Requirement 1: Lookup data (defined in lookup.c) ---------- */
extern const char  specialtyNames[NUM_SPECIALTIES][30];
extern const double specialtyFees[NUM_SPECIALTIES];
extern const int    specialtyTimePerPatient[NUM_SPECIALTIES]; /* minutes */
extern const int    specialtyDailyCap[NUM_SPECIALTIES];
extern int          specialtyQueueCount[NUM_SPECIALTIES];     /* mutable, resets per run */

extern const char  wardNames[NUM_WARDS][30];
extern const double wardDailyRate[NUM_WARDS];
extern const int    wardBedCapacity[NUM_WARDS];

extern int bedOccupancy[NUM_WARDS][MAX_BEDS_PER_WARD]; /* 0 = free, 1 = occupied */

void initBedOccupancy(void);
void printBedStatus(void);
int  findFreeBed(int wardIndex); /* returns bed index (0-based) or -1 if full */

/* ---------- Requirement 2: Patient data (parallel arrays, in patient.c) ---------- */
extern char   patientName[MAX_PATIENTS][NAME_LEN];
extern int    patientAge[MAX_PATIENTS];
extern int    patientUrgency[MAX_PATIENTS];      /* 1,2,3 */
extern int    patientSpecialtyID[MAX_PATIENTS];  /* 1-4 */
extern int    patientAdmitted[MAX_PATIENTS];     /* 1 = yes, 0 = no */
extern int    patientWardID[MAX_PATIENTS];       /* 1-4, 0 if not admitted */
extern int    patientBedIndex[MAX_PATIENTS];     /* 0-based bed slot, -1 if not admitted */
extern int    patientDaysAdmitted[MAX_PATIENTS];
extern double patientFinalBill[MAX_PATIENTS];
extern int    patientCount;

void registerPatient(void);

/* ---------- Requirement 3: Billing (in billing.c) ---------- */
double calcWaitTime(int specialtyIndex);
double calcSurcharge(int urgencyLevel, double baseFee);
double calcWardCost(int daysAdmitted, int wardIndex);
double calcGrossTotal(double baseFee, double surcharge, double wardCost);
double calcDiscount(int age, double grossTotal);
double calcFinalPayable(double grossTotal, double discount);

/* ---------- Requirement 4: Priority sort (in sort.c) ---------- */
void sortByPriority(int order[MAX_PATIENTS]); /* fills order[] with patient indices */

/* ---------- Requirement 5: Display (in display.c) ---------- */
void printBill(int i, double waitTime, double surcharge, double wardCost,
               double grossTotal, double discount, double finalPayable);

/* ---------- Requirement 6: Reports (in report.c) ---------- */
void printSummaryReport(void);

/* ---------- Requirement 7: File I/O (in fileio.c) ---------- */
void saveBedStatus(void);
void loadBedStatus(void);
void appendPatientRecord(int i, double finalPayable);

#endif
