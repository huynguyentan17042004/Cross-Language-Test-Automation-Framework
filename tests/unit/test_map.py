from __future__ import annotations

from collections.abc import Callable
from pathlib import Path

import pytest

from framework.assertions import require_run_success
from framework.models import RunResult


pytestmark = pytest.mark.unit


@pytest.fixture(scope="module")
def map_executable(
    repo_root: Path,
    compile_harness: Callable[..., Path],
) -> Path:
    return compile_harness(
        "map_harness",
        repo_root / "cpp_harness" / "unit" / "map_harness.cpp",
    )


@pytest.mark.parametrize(
    ("element_type", "required_damage", "expected"),
    [
        ("path", None, "PATH,0"),
        ("obstacle", None, "OBSTACLE,0"),
        ("ground-obstacle", 123, "GROUND_OBSTACLE,123"),
    ],
    ids=("path-default", "obstacle-default", "ground-explicit-damage"),
)
def test_map_element_contract(
    map_executable: Path,
    run_binary: Callable[..., RunResult],
    element_type: str,
    required_damage: int | None,
    expected: str,
) -> None:
    arguments: tuple[object, ...] = ("element", element_type)
    if required_damage is not None:
        arguments += (required_damage,)

    result = run_binary(map_executable, *arguments)

    require_run_success(result)
    assert result.stdout == expected
    assert result.stderr == ""


@pytest.mark.parametrize(
    ("row", "col", "expected_damage"),
    [
        (0, 0, 90),
        (0, 1, 229),
        (1, 0, 347),
        (1, 2, 625),
        (3, 4, 517),
    ],
    ids=("origin", "first-column-step", "first-row-step", "threshold-cell", "modulo-cell"),
)
def test_ground_obstacle_required_damage_formula(
    map_executable: Path,
    run_binary: Callable[..., RunResult],
    row: int,
    col: int,
    expected_damage: int,
) -> None:
    result = run_binary(map_executable, "ground-obstacle", row, col)

    require_run_success(result)
    assert result.stdout == f"GROUND_OBSTACLE,{expected_damage}"
    assert result.stderr == ""


@pytest.mark.parametrize(
    ("actor", "damage", "row", "col", "expected"),
    [
        ("fly", 100, 0, 0, "true"),
        ("ground", 100, 0, 0, "true"),
        ("fly", 100, 1, 1, "false"),
        ("ground", 900, 1, 1, "false"),
        ("fly", 0, 1, 2, "true"),
        ("ground", 625, 1, 2, "false"),
        ("ground", 626, 1, 2, "true"),
        ("fly", 100, -1, 0, "false"),
        ("ground", 100, 3, 0, "false"),
    ],
    ids=(
        "fly-path",
        "ground-path",
        "fly-obstacle",
        "ground-obstacle-blocked",
        "fly-ground-obstacle",
        "ground-damage-equal-threshold",
        "ground-damage-above-threshold",
        "negative-row",
        "row-past-map",
    ),
)
def test_map_is_valid_decision_table(
    map_executable: Path,
    run_binary: Callable[..., RunResult],
    actor: str,
    damage: int,
    row: int,
    col: int,
    expected: str,
) -> None:
    result = run_binary(map_executable, "is-valid", actor, damage, row, col)

    require_run_success(result)
    assert result.stdout == expected
    assert result.stderr == ""
