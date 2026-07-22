# Day 13 — Containers and DevSecOps

## Goal

Understand reproducible multi-stage builds, minimal runtime contents, non-root
execution, health checks, configuration, logging, CI, dependency scanning,
secrets, and software bills of materials (SBOMs).

Read the root `Dockerfile` from top to bottom. Separate build stages can contain
compilers and Maven while the runtime stage receives only the executable and
jar. The service runs as an unprivileged user and configuration arrives through
environment variables. Logs go to stdout/stderr for the container platform.

```bash
docker compose build
docker compose up -d
docker compose ps
docker compose logs -f planner-service
docker compose down
```

CI repeats native, Java, and Python tests on every push/PR. A real secure pipeline
would also pin base images by digest, scan source/dependencies/images, generate
an SBOM, sign artifacts, protect branches, and restrict credentials.

Never bake secrets into source, images, build arguments, or sample `.env` files.
Use a secret manager and least-privilege identities. Dependency pinning improves
repeatability but still requires intentional updates for security fixes.

## Practice

1. Explain what each Docker stage contributes to the final image.
2. Inspect the running UID.
3. Break the health endpoint and observe container health.
4. Research Syft (SBOM) and Grype/Trivy (scanning) without adding them yet.

Checklist: I can build the image and explain why compilers are absent at runtime.
