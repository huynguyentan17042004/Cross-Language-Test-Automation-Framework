# Local Verification

Verified on 2026-10-17 in the repository-local virtual environment.

## Environment

- Python: 3.13.13
- Pytest: 8.4.2
- C++ command: `g++` (Apple Clang 21.0.0 on this macOS host)
- SUT snapshot: upstream commit `533d7c150680e40aebb20f7d6b0d0c164b81bf42`

## Results

| Command slice | Result |
|---|---:|
| `python -m pip check` | no broken requirements |
| `python -m pytest --layer framework` | 10 passed |
| `python -m pytest --layer smoke` | 9 passed |
| `python -m pytest --layer unit --junitxml=reports/unit.xml` | 26 passed |
| `python -m pytest --layer structural --junitxml=reports/structural.xml` | 10 passed |
| `python -m pytest --layer integration` | 1 passed |
| `python -m pytest --layer memory` | 1 passed |
| `python -m pytest --layer all` | 48 passed |

The marker totals overlap because every smoke test is also marked `unit`.
The unit report contains Position, MapElement, ground-obstacle damage formula,
and `Map::isValid` decision-table evidence.
The structural report contains ten isolated probes and matching per-probe
compiler diagnostics under `reports/structural-diagnostics/`.
The integration baseline proves cases 19-22 execute successfully; it is not a
claim that every printed value is specification-correct.
