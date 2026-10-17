from __future__ import annotations

from collections.abc import Callable
from pathlib import Path

import pytest

from framework.assertions import require_run_success
from framework.models import RunResult


pytestmark = pytest.mark.integration


@pytest.fixture(scope="module")
def scenario_executable(
    repo_root: Path,
    compile_harness: Callable[..., Path],
) -> Path:
    return compile_harness(
        "integration_scenarios",
        repo_root / "cpp_harness" / "integration" / "scenario_harness.cpp",
    )


def test_flyteam1_moves_right_on_an_empty_map(
    scenario_executable: Path,
    run_binary: Callable[..., RunResult],
    make_config: Callable[..., Path],
    record_integration_run: Callable[[str, RunResult], None],
) -> None:
    config_path = make_config("movement")

    result = run_binary(scenario_executable, "movement", config_path)
    record_integration_run("movement", result)
    require_run_success(result)

    assert result.stdout == "before=(0,0)\nMSG: FlyTeam1 moved\nafter=(0,1)\n"
    assert result.stderr == ""


def test_flyteam1_encounter_defeats_dragonlord(
    scenario_executable: Path,
    run_binary: Callable[..., RunResult],
    make_config: Callable[..., Path],
    record_integration_run: Callable[[str, RunResult], None],
) -> None:
    config_path = make_config(
        "encounter",
        {"DRAGONLORD_INIT_POS": "(0,0)"},
    )

    result = run_binary(scenario_executable, "encounter", config_path)
    record_integration_run("encounter", result)
    require_run_success(result)

    assert result.stdout == (
        "MSG: FlyTeam1 encounters DragonLord\n"
        "dragonlord_defeated=true\n"
    )
    assert result.stderr == ""


def test_program_stops_when_both_flyteams_are_out(
    scenario_executable: Path,
    run_binary: Callable[..., RunResult],
    make_config: Callable[..., Path],
    record_integration_run: Callable[[str, RunResult], None],
) -> None:
    config_path = make_config(
        "stop",
        {
            "FLYTEAM1_INIT_HP": 1,
            "FLYTEAM2_INIT_HP": 1,
        },
    )

    result = run_binary(scenario_executable, "stop", config_path)
    record_integration_run("stop", result)
    require_run_success(result)

    assert result.stdout == "stop=true\n"
    assert result.stderr == ""
