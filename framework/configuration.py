"""Deterministic configuration data builders shared by test layers."""

from __future__ import annotations

from collections.abc import Mapping
from pathlib import Path


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


def write_config(
    path: Path,
    overrides: Mapping[str, object] | None = None,
    *,
    spaced: bool = False,
) -> Path:
    """Write one complete §3.10 configuration and return its path."""

    values = DEFAULT_CONFIG | {
        key: str(value) for key, value in (overrides or {}).items()
    }
    unknown = values.keys() - DEFAULT_CONFIG.keys()
    if unknown:
        names = ", ".join(sorted(unknown))
        raise ValueError(f"Unknown configuration keys: {names}")

    separator = " = " if spaced else "="
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(
        "\n".join(f"{key}{separator}{value}" for key, value in values.items())
        + "\n",
        encoding="utf-8",
    )
    return path
