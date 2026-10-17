from __future__ import annotations

from collections.abc import Callable
from pathlib import Path

import pytest

from framework.assertions import require_run_success
from framework.models import RunResult


pytestmark = [pytest.mark.smoke, pytest.mark.unit]


@pytest.fixture(scope="module")
def position_executable(
    repo_root: Path,
    compile_harness: Callable[..., Path],
) -> Path:
    return compile_harness(
        "position_harness",
        repo_root / "cpp_harness" / "unit" / "position_harness.cpp",
    )


@pytest.mark.parametrize(
    ("arguments", "expected"),
    [
        (("construct-default",), "(0,0)"),
        (("construct-values", 1, 15), "(1,15)"),
        (("construct-values", -1, -1), "(-1,-1)"),
        (("construct-string", "(7,9)"), "(7,9)"),
        (("set-row", 1, 2, 5), "(5,2)"),
        (("set-col", 1, 2, 8), "(1,8)"),
        (("is-equal", 1, 2, 1, 2), "true"),
        (("is-equal", 1, 2, 2, 1), "false"),
    ],
    ids=(
        "default-constructor",
        "value-constructor",
        "npos-boundary",
        "string-constructor",
        "set-row",
        "set-column",
        "equal-position",
        "different-position",
    ),
)
def test_position_scenarios(
    position_executable: Path,
    run_binary: Callable[..., RunResult],
    arguments: tuple[object, ...],
    expected: str,
) -> None:
    result = run_binary(position_executable, *arguments)
    require_run_success(result)
    assert result.stdout == expected
    assert result.stderr == ""


def test_harness_rejects_unknown_scenario(
    position_executable: Path,
    run_binary: Callable[..., RunResult],
) -> None:
    result = run_binary(position_executable, "not-a-scenario")

    assert result.returncode == 64
    assert "unknown scenario" in result.stderr
