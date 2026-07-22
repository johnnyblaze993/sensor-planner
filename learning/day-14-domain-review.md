# Day 14 — Domain vocabulary and final review

## Goal

Use geospatial/radar terminology carefully and demonstrate repository independence.

Read `docs/geospatial-and-radar-glossary.md`. For every quantity ask: units,
reference frame/datum, accuracy, time, assumptions, and valid range. Saying
"altitude 180" is incomplete until the unit and vertical reference are known.

This sandbox implements spherical Haversine distance and a terrain-free radio
horizon approximation. It does not implement DTED lookup, coordinate conversion,
terrain masking, radar equation, probability of detection, signal-to-noise,
tracking, or operational planning.

## Final practical exam

Without copying commands, demonstrate that you can:

1. Explain the three-language request flow.
2. Configure, build, test, and run C++.
3. Set a breakpoint and inspect a target.
4. Add a validated C++ field and unit test.
5. Package Java and add/test a small endpoint.
6. Generate and validate Python scenarios.
7. Build and run the container.
8. Diagnose one intentionally malformed input at each boundary.
9. Define Haversine, CRS, DTED, line of sight, SNR, and radar cross section.
10. Name the project's simplifications without overselling its accuracy.

## Reflection prompts

- Which C++ syntax no longer looks alien?
- Where does every object/resource get cleaned up?
- Which errors are compile, link, runtime, protocol, or domain failures?
- What change can you make independently next week?

Ready means you can navigate and make a controlled change—not that you have
mastered three languages or radar physics in fourteen days.
