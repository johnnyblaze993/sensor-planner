#!/usr/bin/env python3
"""Execute all JSON scenarios, validate results, and print a concise report."""

from __future__ import annotations

import argparse
import json
import statistics
import time
from pathlib import Path

from call_planner_service import call_planner_service
from validate_results import validate_rankings


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("directory", type=Path, nargs="?", default=Path("sample-data/generated"))
    parser.add_argument("--url", default="http://localhost:8080")
    parser.add_argument("--results", type=Path, default=Path("sample-data/results"))
    args = parser.parse_args()

    files = sorted(args.directory.glob("*.json"))
    if not files:
        raise SystemExit(f"No JSON scenarios found in {args.directory}")
    args.results.mkdir(parents=True, exist_ok=True)

    passed = 0
    timings: list[float] = []
    failures: list[str] = []
    for path in files:
        try:
            scenario = json.loads(path.read_text(encoding="utf-8"))
            start = time.perf_counter()
            result = call_planner_service(scenario, args.url)
            timings.append((time.perf_counter() - start) * 1000.0)
            validate_rankings(scenario, result)
            (args.results / path.name).write_text(
                json.dumps(result, indent=2) + "\n", encoding="utf-8"
            )
            passed += 1
        except Exception as error:  # A batch runner should continue after one failure.
            failures.append(f"{path.name}: {error}")

    print(f"Scenarios executed: {len(files)}")
    print(f"Passed: {passed}")
    print(f"Failed: {len(failures)}")
    if timings:
        print(f"Average response time: {statistics.mean(timings):.1f} ms")
    for failure in failures:
        print(f"- {failure}")
    raise SystemExit(0 if not failures else 1)


if __name__ == "__main__":
    main()
