#!/usr/bin/env python3
"""Call the Java REST service with one scenario file."""

from __future__ import annotations

import argparse
import json
from pathlib import Path
from typing import Any

import requests


def call_planner_service(
    scenario: dict[str, Any],
    base_url: str = "http://localhost:8080",
    timeout_seconds: float = 5.0,
) -> dict[str, Any]:
    response = requests.post(
        f"{base_url.rstrip('/')}/api/plans", json=scenario, timeout=timeout_seconds
    )
    # This raises a useful requests.HTTPError for 4xx and 5xx responses.
    response.raise_for_status()
    return response.json()


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("scenario", type=Path)
    parser.add_argument("--url", default="http://localhost:8080")
    args = parser.parse_args()
    scenario = json.loads(args.scenario.read_text(encoding="utf-8"))
    print(json.dumps(call_planner_service(scenario, args.url), indent=2))


if __name__ == "__main__":
    main()
