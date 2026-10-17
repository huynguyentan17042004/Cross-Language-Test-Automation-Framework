from __future__ import annotations

from collections.abc import Callable
from pathlib import Path

import pytest

from framework.assertions import require_run_success
from framework.models import RunResult


pytestmark = pytest.mark.regression


@pytest.mark.parametrize(
    ("actor", "hp", "damage", "expected_hp", "expected_damage"),
    [
        ("fly", -1, -1, 0, 0),
        ("fly", 0, 0, 0, 0),
        ("fly", 500, 900, 500, 900),
        ("fly", 501, 901, 500, 900),
        ("ground", -1, 100, 0, 100),
        ("ground", 100, -1, 100, 0),
        ("ground", 501, 901, 500, 900),
    ],
    ids=(
        "fly-lower-clamp",
        "fly-lower-boundary",
        "fly-upper-boundary",
        "fly-upper-clamp",
        "ground-hp-lower-clamp",
        "ground-damage-lower-clamp",
        "ground-upper-clamp",
    ),
)
def test_warrior_constructor_boundaries(
    domain_executable: Path,
    run_binary: Callable[..., RunResult],
    actor: str,
    hp: int,
    damage: int,
    expected_hp: int,
    expected_damage: int,
) -> None:
    result = run_binary(domain_executable, "warrior", actor, hp, damage)
    require_run_success(result)
    assert result.stdout == (
        f"hp={expected_hp};damage={expected_damage};position=(1,1)\n"
    )
    assert result.stderr == ""


@pytest.mark.parametrize(
    ("hp", "expected"),
    [
        (1, "hp=0;position=(-1,-1)\n"),
        (0, "hp=0;position=(-1,-1)\n"),
        (2, "hp=2;position=(1,1)\n"),
        (501, "hp=500;position=(1,1)\n"),
    ],
    ids=("exhausted-at-one", "exhausted-at-zero", "active-above-one", "setter-cap"),
)
def test_warrior_hp_state_transition(
    domain_executable: Path,
    run_binary: Callable[..., RunResult],
    hp: int,
    expected: str,
) -> None:
    result = run_binary(domain_executable, "set-hp", hp)
    require_run_success(result)
    assert result.stdout == expected
    assert result.stderr == ""


@pytest.mark.parametrize(
    ("actor", "rule", "rows", "cols", "row", "col", "expected"),
    [
        ("fly", "R", 3, 3, 1, 1, "(1,2)"),
        ("fly", "R", 3, 3, 1, 2, "(1,1)"),
        ("fly", "U", 1, 1, 0, 0, "(0,0)"),
        ("ground", "D", 3, 3, 1, 1, "(2,1)"),
        ("ground", "L", 3, 3, 1, 0, "(1,1)"),
        ("ground", "U", 1, 1, 0, 0, "(0,0)"),
    ],
    ids=(
        "fly-normal",
        "fly-opposite-fallback",
        "fly-blocked-both-directions",
        "ground-normal",
        "ground-opposite-fallback",
        "ground-blocked-both-directions",
    ),
)
def test_movement_decision_table(
    domain_executable: Path,
    run_binary: Callable[..., RunResult],
    actor: str,
    rule: str,
    rows: int,
    cols: int,
    row: int,
    col: int,
    expected: str,
) -> None:
    result = run_binary(
        domain_executable,
        "movement",
        actor,
        rule,
        rows,
        cols,
        row,
        col,
        900,
    )
    require_run_success(result)
    assert result.stdout.splitlines()[-1] == f"position={expected}"
    assert result.stderr == ""
