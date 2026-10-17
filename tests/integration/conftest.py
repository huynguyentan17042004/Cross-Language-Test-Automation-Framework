"""Fixtures for isolated configuration-driven integration scenarios."""

from __future__ import annotations

from collections.abc import Callable, Mapping
from pathlib import Path

import pytest

from framework.configuration import write_config
from framework.diagnostics import write_run_diagnostics
from framework.models import RunResult


@pytest.fixture
def make_config(tmp_path: Path) -> Callable[..., Path]:
    """Create a complete §3.10 configuration without machine-specific paths."""

    def create(
        name: str,
        overrides: Mapping[str, object] | None = None,
        *,
        spaced: bool = False,
    ) -> Path:
        return write_config(
            tmp_path / f"{name}.config",
            overrides,
            spaced=spaced,
        )

    return create


@pytest.fixture(scope="session")
def integration_diagnostics_dir(repo_root: Path) -> Path:
    path = repo_root / "reports" / "integration-diagnostics"
    path.mkdir(parents=True, exist_ok=True)
    return path


@pytest.fixture
def record_integration_run(
    integration_diagnostics_dir: Path,
) -> Callable[[str, RunResult], None]:
    """Persist process evidence so CI can upload it even after an assertion fails."""

    def record(name: str, result: RunResult) -> None:
        write_run_diagnostics(integration_diagnostics_dir, name, result)

    return record
