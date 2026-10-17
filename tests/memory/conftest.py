from __future__ import annotations

from collections.abc import Callable
from pathlib import Path

import pytest

from framework.diagnostics import write_run_diagnostics
from framework.models import RunResult


@pytest.fixture(scope="session")
def memory_diagnostics_dir(repo_root: Path) -> Path:
    path = repo_root / "reports" / "memory-diagnostics"
    path.mkdir(parents=True, exist_ok=True)
    return path


@pytest.fixture
def record_memory_run(
    memory_diagnostics_dir: Path,
) -> Callable[[str, RunResult], None]:
    def record(name: str, result: RunResult) -> None:
        write_run_diagnostics(memory_diagnostics_dir, name, result)

    return record
