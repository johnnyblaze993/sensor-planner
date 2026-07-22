# Day 9 — Implementing the C++ planner

## Goal

Read the integrated models, distance formula, scoring, and ranking as one flow.

Start at `main.cpp`, then jump through declarations rather than reading files
alphabetically:

```text
main
 -> parseJson/readRequestFile
 -> Planner::createPlan
    -> calculateDistanceKm
    -> hasRadioLineOfSight
    -> calculateScore
    -> std::sort
 -> toJson
```

Haversine converts degrees to radians because `sin` and `cos` expect radians.
It treats Earth as a sphere of radius 6371 km. Floating-point comparisons need
tolerances; the test checks a sensible interval, not exact decimal identity.

The score components are normalized and weighted. `std::clamp` enforces the
documented range. Deterministic ID tie-breaking makes repeated tests reliable.

## Practice

1. Calculate the same-position case by hand.
2. Temporarily change distance weight and predict which test could change.
3. Add a third target with the same coordinate and different ID; inspect ties.
4. Add `explanation` fields to the model on a branch, following the value across
   C++, JSON, Java, Python validation, and tests.

Checklist: I can explain every transformation from request to ranked result.
