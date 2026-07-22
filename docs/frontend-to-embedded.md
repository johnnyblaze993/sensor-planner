# From frontend development to embedded/native software

Your transferable skills are substantial: data flow, API contracts, modularity,
automated testing, version control, and reading logs all still matter.

| Frontend idea | Native/service analogue |
|---|---|
| TypeScript interface | C++ struct/header or Java record |
| API route | Spring controller |
| Package build | Compilation plus linking |
| Browser runtime error | Native crash or Java exception |
| DevTools call stack | GDB/JDB stack trace |
| npm dependency graph | Maven/CMake dependency graph |
| Component boundary | Module, library, process, or service boundary |
| Garbage collection | Java/Python GC; C++ lifetime and RAII |

The largest mental shift is object lifetime. JavaScript objects generally live
until garbage collection. A local C++ object is destroyed deterministically
when its scope ends. Containers and smart pointers use RAII so cleanup follows
that same rule.

Embedded work also makes resource constraints, binary formats, hardware I/O,
determinism, cross-compilation, and undefined behavior more visible. This
project does not emulate hardware, but its native core, file formats, process
contract, and debugging practice are a useful bridge.
