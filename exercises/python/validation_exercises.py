"""Run with: python -m pytest validation_exercises.py"""

import pytest


def require_unique_ids(targets: list[dict]) -> None:
    """Exercise: raise ValueError when an ID occurs more than once."""
    raise NotImplementedError("TODO")


def test_unique_ids_pass() -> None:
    require_unique_ids([{"id": "a"}, {"id": "b"}])


def test_duplicate_ids_fail() -> None:
    with pytest.raises(ValueError, match="duplicate"):
        require_unique_ids([{"id": "a"}, {"id": "a"}])
