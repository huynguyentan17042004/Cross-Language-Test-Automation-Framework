from __future__ import annotations

from collections.abc import Callable
from pathlib import Path

import pytest

from framework.assertions import require_run_success
from framework.models import RunResult


pytestmark = pytest.mark.regression


@pytest.mark.parametrize(
    ("kind", "actor", "hp", "damage", "expected"),
    [
        ("scale", "fly", 300, 400, "usable=true;hp=300;damage=500"),
        ("scale", "fly", 300, 401, "usable=false;hp=300;damage=401"),
        ("herb", "fly", 100, 300, "usable=true;hp=120;damage=300"),
        ("herb", "fly", 101, 300, "usable=false;hp=101;damage=300"),
        ("trap", "fly", 300, 300, "usable=false;hp=300;damage=300"),
        (
            "trap",
            "ground",
            300,
            300,
            "usable=true;hp=300;damage=300;trap_turns=4",
        ),
    ],
    ids=(
        "scale-at-boundary",
        "scale-above-boundary",
        "herb-at-boundary",
        "herb-above-boundary",
        "trap-rejects-flyteam",
        "trap-extends-groundteam",
    ),
)
def test_item_decision_table(
    domain_executable: Path,
    run_binary: Callable[..., RunResult],
    kind: str,
    actor: str,
    hp: int,
    damage: int,
    expected: str,
) -> None:
    result = run_binary(domain_executable, "item", kind, actor, hp, damage)
    require_run_success(result)
    assert result.stdout.splitlines()[-1] == expected
    assert result.stderr == ""


@pytest.mark.parametrize(
    ("actor", "attempts", "expected_accepted", "expected_count"),
    [
        ("fly", 5, 5, 5),
        ("fly", 6, 5, 5),
        ("ground", 7, 7, 7),
        ("ground", 8, 7, 7),
    ],
    ids=("fly-at-capacity", "fly-rejects-sixth", "ground-at-capacity", "ground-rejects-eighth"),
)
def test_bag_capacity(
    domain_executable: Path,
    run_binary: Callable[..., RunResult],
    actor: str,
    attempts: int,
    expected_accepted: int,
    expected_count: int,
) -> None:
    result = run_binary(domain_executable, "bag-capacity", actor, attempts)
    require_run_success(result)
    assert result.stdout.startswith(
        f"accepted={expected_accepted};Bag[count={expected_count};"
    )
    assert result.stderr == ""


def test_bag_inserts_at_head(
    domain_executable: Path,
    run_binary: Callable[..., RunResult],
) -> None:
    result = run_binary(domain_executable, "bag-order")
    require_run_success(result)
    assert result.stdout == (
        "Bag[count=3;TrapEnhancer,HealingHerb,DragonScale]\n"
    )
    assert result.stderr == ""


def test_bag_get_by_type(
    domain_executable: Path,
    run_binary: Callable[..., RunResult],
) -> None:
    result = run_binary(domain_executable, "bag-get-type")
    require_run_success(result)
    assert result.stdout == (
        "selected=HealingHerb;Bag[count=2;TrapEnhancer,DragonScale]\n"
    )
    assert result.stderr == ""


def test_bag_get_first_usable_item(
    domain_executable: Path,
    run_binary: Callable[..., RunResult],
) -> None:
    result = run_binary(domain_executable, "bag-get-usable")
    require_run_success(result)
    assert result.stdout == "selected=HealingHerb;Bag[count=1;DragonScale]\n"
    assert result.stderr == ""


@pytest.mark.parametrize(
    ("capacity", "attempts", "expected"),
    [
        (2, 1, "accepted=1;size=1;full=false;negative_is_null=true\n"),
        (2, 2, "accepted=11;size=2;full=true;negative_is_null=true\n"),
        (2, 3, "accepted=110;size=2;full=true;negative_is_null=true\n"),
    ],
    ids=("below-capacity", "at-capacity", "rejects-over-capacity"),
)
def test_array_moving_object_capacity(
    domain_executable: Path,
    run_binary: Callable[..., RunResult],
    capacity: int,
    attempts: int,
    expected: str,
) -> None:
    result = run_binary(domain_executable, "array", capacity, attempts)
    require_run_success(result)
    assert result.stdout == expected
    assert result.stderr == ""
