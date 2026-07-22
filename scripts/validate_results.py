#!/usr/bin/env python3
"""Validate structural and ranking invariants in a planner response."""

from __future__ import annotations

import argparse
import json
import math
from pathlib import Path
from typing import Any


class ValidationError(ValueError):
    """Raised when a planner response violates its expected contract."""


def validate_rankings(scenario: dict[str, Any], result: dict[str, Any]) -> None:
    ranked = result.get("rankedTargets")
    if not isinstance(ranked, list):
        raise ValidationError("rankedTargets must be a list")

    expected_ids = {target["id"] for target in scenario["targets"]}
    actual_ids = [target.get("id") for target in ranked]
    if len(actual_ids) != len(set(actual_ids)):
        raise ValidationError("result contains duplicate target IDs")
    if set(actual_ids) != expected_ids:
        raise ValidationError("result must contain every input target exactly once")

    previous_score = math.inf
    for index, target in enumerate(ranked):
        score = target.get("score")
        distance = target.get("distanceKm")
        if not isinstance(score, (int, float)) or not math.isfinite(score):
            raise ValidationError(f"target {index} has an invalid score")
        if not 0.0 <= score <= 1.0:
            raise ValidationError(f"target {index} score is outside 0..1")
        if score > previous_score:
            raise ValidationError("targets are not sorted by descending score")
        if not isinstance(distance, (int, float)) or distance < 0 or not math.isfinite(distance):
            raise ValidationError(f"target {index} has an invalid distance")
        if not isinstance(target.get("visible"), bool):
            raise ValidationError(f"target {index} has no boolean visible value")
        previous_score = score


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("scenario", type=Path)
    parser.add_argument("result", type=Path)
    args = parser.parse_args()
    scenario = json.loads(args.scenario.read_text(encoding="utf-8"))
    result = json.loads(args.result.read_text(encoding="utf-8"))
    validate_rankings(scenario, result)
    print("Result is valid")


if __name__ == "__main__":
    main()
