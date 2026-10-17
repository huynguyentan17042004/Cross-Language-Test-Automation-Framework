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
| `python -m pytest -q -m framework` | 10 passed |
| `python -m pytest -q -m smoke` | 9 passed |
| `python -m pytest -q -m structural` | 2 passed |
| `python -m pytest -q -m integration` | 1 passed |
| `python -m pytest -q -m memory` | 1 passed |
| `python -m pytest -q --junitxml=reports/pytest.xml` | 23 passed |

The marker totals overlap because every smoke test is also marked `unit`.
The integration baseline proves cases 19-22 execute successfully; it is not a
claim that every printed value is specification-correct.

