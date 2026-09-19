# Smart Hospital & Resource Allocation System

A modular, menu-driven C console application for CSC 1012 (Introduction to
Computer Programming), simulating patient intake, bed allocation, emergency
triage priority sorting, and medical billing for a small hospital.

## Features
- Pre-defined lookup tables for doctor specialties and hospital wards
- 2D bed occupancy matrix (`bedOccupancy[4][20]`) tracking free/occupied beds per ward
- Patient registration using parallel arrays (no structs)
- Automatic billing: consultation fee, emergency surcharge, ward stay cost,
  age subsidy discount, final payable amount
- Estimated waiting time based on specialty queue length
- Priority sorting of patients (Critical > Urgent > Normal, stable by registration order)
- Summary/analytics report: patient counts by urgency, total revenue, total
  discounts, bed occupancy %, highest-paying patient
- File handling: bed status persists between runs (`beds_status.txt`), every
  patient is logged permanently (`patient_records.txt`)

## Project structure
```
include/
  hospital.h      - constants, lookup table declarations, function prototypes
src/
  lookup.c        - Requirement 1: lookup tables & bed occupancy matrix
  patient.c       - Requirement 2: patient parallel arrays & registration
  billing.c       - Requirement 3: billing & waiting time formulas
  sort.c          - Requirement 4: priority sort
  display.c       - Requirement 5: formatted bill printout
  report.c        - Requirement 6: summary/analytics report
  fileio.c        - Requirement 7: file handling (bonus)
  main.c          - menu loop tying everything together
```

## Building
Open `smart-hospital-system.cbp` in Code::Blocks and Build and Run (F9),
or from the command line:
```
gcc -Wall -Iinclude -o smart_hospital src/*.c
./smart_hospital
```

## Assumptions
- Estimated wait time uses the specialty's queue count *after* the current
  patient is added to it (queue count is incremented as soon as a specialty
  is selected during registration).
- Critical (Level 3) patients are always shown as receiving immediate
  attention (0.00 min wait) on their printed bill, regardless of the queue
  formula, since they are triaged ahead of the queue.
- The daily patient cap in the specialty lookup table is stored for
  reference but not actively enforced as a hard registration limit, since
  the assignment brief does not require blocking registration once a
  specialty's cap is reached.
- Age subsidy discount boundaries are strict: exactly age 5 and exactly
  age 65 do NOT receive the discount (only `age < 5` or `age > 65` do),
  matching the formula as written in the spec.
