# Day 10 — JSON, CSV, and XML ingestion

## Goal

Treat parsing as a trust boundary: files may be absent, malformed, incomplete,
or numerically invalid.

```bash
planner-core/build/planner-core --input planner-core/sample-data/scenario.json
planner-core/build/planner-core --input planner-core/sample-data/scenario.csv
planner-core/build/planner-core --input planner-core/sample-data/scenario.xml
```

JSON uses a mature library and catches its parse/type exceptions. The CSV and
XML readers intentionally support only the included learning formats. Their
limitations are documented because silent assumptions become defects.

Validation occurs after parsing in `Planner`: latitude `-90..90`, longitude
`-180..180`, finite numbers, non-negative altitude, targets present, IDs present,
and IDs unique. Java repeats input validation because C++ remains independently
callable.

Production XML must use a proper parser configured against external-entity and
expansion attacks. Production CSV must define quoting, encoding, headers, and
large-file behavior. Binary formats additionally define byte order and sizes.

## Practice

1. Test a nonexistent file and inspect stderr plus exit status.
2. Remove a required JSON field.
3. Add a CSV row with too few columns.
4. Make an XML target omit `id`.
5. Write a unit test for each failure before modifying parser behavior.

Checklist: I can distinguish parsing, structural validation, and domain validation.
