from __future__ import annotations

from collections.abc import Callable
from pathlib import Path

import pytest

from framework.assertions import require_run_success
from framework.models import RunResult


pytestmark = pytest.mark.regression


@pytest.mark.parametrize(
    ("fly1", "fly2", "expected"),
    [
        ((0, 0), (3, 4), "(4,3)"),
        ((0, 5), (1, 5), "(0,1)"),
        ((0, 6), (1, 6), "(1,0)"),
        ((9, 9), (0, 0), "(9,9)"),
    ],
    ids=(
        "near-team-swaps",
        "distance-five-swaps",
        "distance-six-keeps-order",
        "both-far-keeps-order",
    ),
)
def test_dragonlord_next_position(
    domain_executable: Path,
    run_binary: Callable[..., RunResult],
    fly1: tuple[int, int],
    fly2: tuple[int, int],
    expected: str,
) -> None:
    result = run_binary(
        domain_executable,
        "lord-next",
        *fly1,
        *fly2,
    )
    require_run_success(result)
    assert result.stdout == f"next={expected}\n"
    assert result.stderr == ""


@pytest.mark.parametrize(
    ("dragon_type", "start", "target", "expected"),
    [
        ("SD1", (3, 3), (0, 3), "(2,3)"),
        ("SD1", (3, 3), (3, 6), "(3,4)"),
        ("SD2", (3, 3), (6, 3), "(4,3)"),
        ("SD2", (3, 3), (3, 0), "(3,2)"),
        ("SD1", (3, 3), (2, 4), "(2,3)"),
        ("SD3", (3, 3), (0, 0), "(-1,-1)"),
    ],
    ids=("up", "right", "down", "left", "tie-prefers-up", "sd3-stationary"),
)
def test_smartdragon_movement_priority(
    domain_executable: Path,
    run_binary: Callable[..., RunResult],
    dragon_type: str,
    start: tuple[int, int],
    target: tuple[int, int],
    expected: str,
) -> None:
    result = run_binary(
        domain_executable,
        "smart-next",
        dragon_type,
        *start,
        *target,
        100,
    )
    require_run_success(result)
    assert result.stdout == f"next={expected}\n"
    assert result.stderr == ""


@pytest.mark.parametrize(
    ("successful_moves", "created"),
    [(0, False), (4, False), (5, True), (6, False), (10, True)],
    ids=("none", "before-five", "at-five", "after-five", "at-ten"),
)
def test_smartdragon_spawn_interval(
    domain_executable: Path,
    run_binary: Callable[..., RunResult],
    successful_moves: int,
    created: bool,
) -> None:
    result = run_binary(domain_executable, "spawn", successful_moves)
    require_run_success(result)
    assert result.stdout.startswith(f"created={str(created).lower()}")
    if created:
        assert ";type=smartdragon[" in result.stdout
    assert result.stderr == ""
