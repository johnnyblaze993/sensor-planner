"""Shared data models for the Python side of Sensor Planning Sandbox."""

from __future__ import annotations

from dataclasses import asdict, dataclass
from typing import Any


@dataclass(frozen=True)
class Coordinate:
    """An immutable geographic position in decimal degrees and meters."""

    latitude: float
    longitude: float
    altitudeMeters: float


@dataclass(frozen=True)
class Target:
    id: str
    latitude: float
    longitude: float
    altitudeMeters: float


@dataclass(frozen=True)
class Scenario:
    sensor: Coordinate
    targets: list[Target]

    def to_dict(self) -> dict[str, Any]:
        """Convert nested dataclasses into JSON-compatible dictionaries."""
        return asdict(self)
