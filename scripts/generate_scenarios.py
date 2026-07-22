#!/usr/bin/env python3
"""Generate repeatable, synthetic sensor-planning inputs.

Run ``python generate_scenarios.py --count 10 --seed 42`` from this folder.
No coordinates in this project represent operational or sensitive data.
"""

from __future__ import annotations

import argparse
import json
import random
import xml.etree.ElementTree as ET
from pathlib import Path

from scenario_models import Coordinate, Scenario, Target


def generate_scenario(random_source: random.Random, target_count: int = 5) -> Scenario:
    """Create targets near a synthetic sensor centered around Huntsville, AL."""
    sensor = Coordinate(
        latitude=random_source.uniform(34.5, 34.9),
        longitude=random_source.uniform(-86.9, -86.4),
        altitudeMeters=random_source.uniform(100.0, 450.0),
    )
    targets = [
        Target(
            id=f"target-{index + 1}",
            latitude=sensor.latitude + random_source.uniform(-0.7, 0.7),
            longitude=sensor.longitude + random_source.uniform(-0.7, 0.7),
            altitudeMeters=random_source.uniform(50.0, 700.0),
        )
        for index in range(target_count)
    ]
    return Scenario(sensor=sensor, targets=targets)


def write_json(scenario: Scenario, path: Path) -> None:
    path.write_text(json.dumps(scenario.to_dict(), indent=2) + "\n", encoding="utf-8")


def write_xml(scenario: Scenario, path: Path) -> None:
    root = ET.Element("scenario")
    ET.SubElement(root, "sensor", {key: str(value) for key, value in scenario.to_dict()["sensor"].items()})
    targets = ET.SubElement(root, "targets")
    for target in scenario.targets:
        ET.SubElement(targets, "target", {key: str(value) for key, value in target.__dict__.items()})
    ET.indent(root)
    ET.ElementTree(root).write(path, encoding="unicode", xml_declaration=True)


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--count", type=int, default=10)
    parser.add_argument("--targets", type=int, default=5, help="targets per scenario")
    parser.add_argument("--seed", type=int, default=42, help="same seed means same data")
    parser.add_argument("--output", type=Path, default=Path("sample-data/generated"))
    parser.add_argument("--xml", action="store_true", help="also write XML copies")
    return parser.parse_args()


def main() -> None:
    args = parse_args()
    if args.count < 1 or args.targets < 1:
        raise SystemExit("--count and --targets must be positive")
    args.output.mkdir(parents=True, exist_ok=True)
    random_source = random.Random(args.seed)  # Local generator avoids global state.
    for index in range(args.count):
        scenario = generate_scenario(random_source, args.targets)
        write_json(scenario, args.output / f"scenario-{index + 1:02}.json")
        if args.xml:
            write_xml(scenario, args.output / f"scenario-{index + 1:02}.xml")
    print(f"Generated {args.count} scenarios in {args.output}")


if __name__ == "__main__":
    main()
