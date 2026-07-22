# Day 6 — Java refresh

## Goal

Review classes, records, interfaces, collections, generics, exceptions, streams,
and dependency injection in the context of the service.

```java
public record Coordinate(
        Double latitude,
        Double longitude,
        Double altitudeMeters) {}

public interface PlannerService {
    PlanResult generatePlan(PlanRequest request);
}
```

Records are immutable data carriers. Their accessors are `latitude()`, not
`getLatitude()`. Interfaces state capabilities; `DefaultPlannerService`
implements the use case. `List<T>` is a generic type, so the compiler knows what
elements it contains.

Spring creates annotated components and supplies constructor dependencies.
This is dependency injection: the class requests what it needs rather than
constructing hard-coded collaborators.

Checked exceptions must be declared or caught; runtime exceptions do not.
`CppPlannerClient` translates low-level I/O and interruption problems into one
domain-specific runtime exception while preserving the cause.

## Practice

1. Trace a `PlanRequest` from JSON fields to record accessors.
2. Implement a fake `NativePlannerClient` returning a fixed result.
3. Convert a list of target IDs with a stream, then with a loop; compare clarity.
4. Explain why constructor injection makes tests easier.

Checklist: I can navigate packages and tell records, interfaces, and classes apart.
