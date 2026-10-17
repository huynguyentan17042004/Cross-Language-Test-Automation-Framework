from __future__ import annotations

from collections.abc import Callable
from pathlib import Path

import pytest

from framework.assertions import require_run_success
from framework.models import RunResult


pytestmark = pytest.mark.integration


def test_configuration_parses_all_keys_and_formats_exactly(
    repo_root: Path,
    compile_harness: Callable[..., Path],
    run_binary: Callable[..., RunResult],
    make_config: Callable[..., Path],
    record_integration_run: Callable[[str, RunResult], None],
) -> None:
    executable = compile_harness(
        "integration_scenarios",
        repo_root / "cpp_harness" / "integration" / "scenario_harness.cpp",
    )
    config_path = make_config("all-keys-with-whitespace", spaced=True)

    result = run_binary(executable, "config-str", config_path)
    record_integration_run("configuration", result)
    require_run_success(result)

    expected = """Configuration[
MAP_NUM_ROWS=5
MAP_NUM_COLS=5
MAX_NUM_MOVING_OBJECTS=5
NUM_OBSTACLE=0
ARRAY_OBSTACLE=[]
NUM_GROUND_OBSTACLE=0
ARRAY_GROUND_OBSTACLE=[]
FLYTEAM1_MOVING_RULE=R
FLYTEAM1_INIT_POS=(0,0)
FLYTEAM1_INIT_HP=300
FLYTEAM1_INIT_DAMAGE=100
FLYTEAM2_MOVING_RULE=L
FLYTEAM2_INIT_POS=(4,4)
FLYTEAM2_INIT_HP=300
FLYTEAM2_INIT_DAMAGE=100
GROUNDTEAM_MOVING_RULE=U
GROUNDTEAM_INIT_POS=(3,3)
GROUNDTEAM_INIT_HP=500
GROUNDTEAM_INIT_DAMAGE=50
DRAGONLORD_INIT_POS=(2,2)
NUM_STEPS=1
]
"""
    assert result.stdout == expected
    assert result.stderr == ""
