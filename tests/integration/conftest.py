"""Fixtures for isolated configuration-driven integration scenarios."""

from __future__ import annotations

from collections.abc import Callable, Mapping
from pathlib import Path

import pytest

from framework.models import RunResult


DEFAULT_CONFIG: dict[str, str] = {
    "MAP_NUM_ROWS": "5",
    "MAP_NUM_COLS": "5",
    "MAX_NUM_MOVING_OBJECTS": "5",
    "NUM_OBSTACLE": "0",
    "ARRAY_OBSTACLE": "[]",
    "NUM_GROUND_OBSTACLE": "0",
    "ARRAY_GROUND_OBSTACLE": "[]",
    "FLYTEAM1_MOVING_RULE": "R",
    "FLYTEAM1_INIT_POS": "(0,0)",
    "FLYTEAM1_INIT_HP": "300",
    "FLYTEAM1_INIT_DAMAGE": "100",
    "FLYTEAM2_MOVING_RULE": "L",
    "FLYTEAM2_INIT_POS": "(4,4)",
    "FLYTEAM2_INIT_HP": "300",
    "FLYTEAM2_INIT_DAMAGE": "100",
    "GROUNDTEAM_MOVING_RULE": "U",
    "GROUNDTEAM_INIT_POS": "(3,3)",
    "GROUNDTEAM_INIT_HP": "500",
    "GROUNDTEAM_INIT_DAMAGE": "50",
    "DRAGONLORD_INIT_POS": "(2,2)",
    "NUM_STEPS": "1",
}


@pytest.fixture
def make_config(tmp_path: Path) -> Callable[..., Path]:
    """Create a complete §3.10 configuration without machine-specific paths."""

    def create(
        name: str,
        overrides: Mapping[str, object] | None = None,
        *,
        spaced: bool = False,
    ) -> Path:
        values = DEFAULT_CONFIG | {
            key: str(value) for key, value in (overrides or {}).items()
        }
        separator = " = " if spaced else "="
        path = tmp_path / f"{name}.config"
        path.write_text(
            "\n".join(f"{key}{separator}{value}" for key, value in values.items())
            + "\n",
            encoding="utf-8",
        )
        return path

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
        stem = integration_diagnostics_dir / name
        stem.with_suffix(".stdout.txt").write_text(result.stdout, encoding="utf-8")
        stem.with_suffix(".stderr.txt").write_text(result.stderr, encoding="utf-8")
        stem.with_suffix(".run.txt").write_text(
            "\n".join(
                (
                    f"command={' '.join(result.command)}",
                    f"returncode={result.returncode}",
                    f"timed_out={result.timed_out}",
                    f"duration_seconds={result.duration_seconds:.6f}",
                )
            )
            + "\n",
            encoding="utf-8",
        )

    return record
