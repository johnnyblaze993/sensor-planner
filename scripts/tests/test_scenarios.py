import random

import pytest

from binary_record_example import BinarySensorRecord, decode, encode
from generate_scenarios import generate_scenario
from validate_results import ValidationError, validate_rankings


def test_generation_is_repeatable() -> None:
    assert generate_scenario(random.Random(42)) == generate_scenario(random.Random(42))


def test_valid_rankings_are_accepted() -> None:
    scenario = {"targets": [{"id": "a"}, {"id": "b"}]}
    result = {"rankedTargets": [
        {"id": "a", "distanceKm": 5.0, "score": 0.9, "visible": True},
        {"id": "b", "distanceKm": 9.0, "score": 0.7, "visible": False},
    ]}
    validate_rankings(scenario, result)


def test_out_of_order_scores_are_rejected() -> None:
    scenario = {"targets": [{"id": "a"}, {"id": "b"}]}
    result = {"rankedTargets": [
        {"id": "a", "distanceKm": 5.0, "score": 0.5, "visible": True},
        {"id": "b", "distanceKm": 9.0, "score": 0.7, "visible": False},
    ]}
    with pytest.raises(ValidationError, match="descending"):
        validate_rankings(scenario, result)


def test_binary_record_round_trip() -> None:
    record = BinarySensorRecord(1, 34.7, -86.6, 180.0)
    assert decode(encode(record)) == record
