# Day 8 — Python refresh

## Goal

Review functions, dataclasses, type hints, collections, JSON/XML/files, paths,
arguments, virtual environments, pytest, HTTP, and binary `struct` basics.

```python
from dataclasses import dataclass
from pathlib import Path
import json

@dataclass(frozen=True)
class Coordinate:
    latitude: float
    longitude: float
    altitude_meters: float

payload = json.loads(Path("scenario.json").read_text(encoding="utf-8"))
```

Type hints improve tools and communication but are not automatically enforced
at runtime. `frozen=True` prevents normal reassignment. `pathlib` represents
paths as values rather than fragile string concatenations.

Create an isolated environment:

```bash
cd scripts
python3 -m venv .venv
source .venv/bin/activate
python -m pip install -r requirements-dev.txt
pytest -q
```

Read `binary_record_example.py`. `<Iddd` specifies little-endian bytes: one
unsigned 32-bit integer and three doubles. Binary formats require an exact
schema, byte order, sizes, and validation.

## Practice

1. Generate two scenarios with the same seed and compare them.
2. Add a `--targets` value of 1 and inspect the JSON/XML.
3. Corrupt one binary byte and observe that structure may still decode but data
   integrity is not guaranteed; research checksums conceptually.
4. Add a pytest case for a missing result ID.

Checklist: I can use a venv, read/write JSON, call HTTP, and run pytest.
