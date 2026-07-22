#!/usr/bin/env python3
"""Tiny binary-ingestion lesson using Python's struct module."""

from __future__ import annotations

import struct
from dataclasses import dataclass

# < means little-endian; I is uint32; d is an 8-byte floating-point number.
RECORD_FORMAT = "<Iddd"


@dataclass(frozen=True)
class BinarySensorRecord:
    sensor_id: int
    latitude: float
    longitude: float
    altitude_meters: float


def encode(record: BinarySensorRecord) -> bytes:
    return struct.pack(
        RECORD_FORMAT,
        record.sensor_id,
        record.latitude,
        record.longitude,
        record.altitude_meters,
    )


def decode(data: bytes) -> BinarySensorRecord:
    expected_size = struct.calcsize(RECORD_FORMAT)
    if len(data) != expected_size:
        raise ValueError(f"expected {expected_size} bytes, received {len(data)}")
    return BinarySensorRecord(*struct.unpack(RECORD_FORMAT, data))


if __name__ == "__main__":
    original = BinarySensorRecord(7, 34.725, -86.650, 180.0)
    encoded = encode(original)
    print(f"Encoded bytes ({len(encoded)}): {encoded.hex()}")
    print(f"Decoded record: {decode(encoded)}")
