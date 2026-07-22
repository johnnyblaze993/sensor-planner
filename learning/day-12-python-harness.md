# Day 12 — Python scenario harness

## Goal

Run a repeatable experiment, preserve outputs, validate invariants, and summarize
failures without stopping after the first scenario.

```bash
cd scripts
python generate_scenarios.py --count 10 --targets 5 --seed 42 --xml
python run_scenarios.py --url http://localhost:8080
```

A local `random.Random(seed)` avoids changing global random state. Repeatability
is essential when reproducing a failure. The batch runner uses a monotonic clock
for elapsed time and saves valid responses beside a concise summary.

The validator checks score range/order, distance validity, boolean visibility,
and exact ID preservation. It does not duplicate Haversine/scoring. Independent
properties are more likely to reveal an implementation defect.

## Practice

1. Generate 100 scenarios and inspect timing distribution.
2. Duplicate an input ID and see which boundary catches it.
3. Modify a saved result to lose one target and run `validate_results.py`.
4. Add minimum/maximum response time to the summary.
5. Add a test using a mocked `requests.post` failure.

Checklist: I can make a failing batch repeat with its seed and saved input.
