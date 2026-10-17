from __future__ import annotations

from collections.abc import Callable
from pathlib import Path

import pytest

from framework.assertions import require_run_success
from framework.configuration import write_config
from framework.models import RunResult
from framework.sanitizers import ASAN_COMPILE_FLAGS, asan_runtime_env


pytestmark = [pytest.mark.memory, pytest.mark.slow]


def assert_clean_asan(result: RunResult) -> None:
    require_run_success(result)
    assert "ERROR: AddressSanitizer" not in result.stderr
    assert "LeakSanitizer" not in result.stderr


@pytest.fixture(scope="module")
def domain_asan(
    repo_root: Path,
    compile_harness: Callable[..., Path],
) -> Path:
    return compile_harness(
        "domain_harness_asan",
        repo_root / "cpp_harness" / "unit" / "domain_harness.cpp",
        extra_flags=ASAN_COMPILE_FLAGS,
    )


def test_map_lifecycle_under_address_sanitizer(
    repo_root: Path,
    compile_harness: Callable[..., Path],
    run_binary: Callable[..., RunResult],
    record_memory_run: Callable[[str, RunResult], None],
) -> None:
    executable = compile_harness(
        "map_harness_asan",
        repo_root / "cpp_harness" / "unit" / "map_harness.cpp",
        extra_flags=ASAN_COMPILE_FLAGS,
    )
    result = run_binary(
        executable,
        "ground-obstacle",
        3,
        4,
        env=asan_runtime_env(),
    )
    record_memory_run("map", result)
    assert_clean_asan(result)


@pytest.mark.parametrize(
    ("name", "arguments"),
    [
        ("bag", ("bag-capacity", "ground", 8)),
        ("smartdragon", ("smart-next", "SD1", 3, 3, 0, 3, 100)),
        ("array", ("array", 2, 3)),
    ],
)
def test_domain_object_lifecycle_under_address_sanitizer(
    name: str,
    arguments: tuple[object, ...],
    domain_asan: Path,
    run_binary: Callable[..., RunResult],
    record_memory_run: Callable[[str, RunResult], None],
) -> None:
    result = run_binary(domain_asan, *arguments, env=asan_runtime_env())
    record_memory_run(name, result)
    assert_clean_asan(result)


def test_configuration_and_program_lifecycle_under_address_sanitizer(
    tmp_path: Path,
    repo_root: Path,
    compile_harness: Callable[..., Path],
    run_binary: Callable[..., RunResult],
    record_memory_run: Callable[[str, RunResult], None],
) -> None:
    executable = compile_harness(
        "scenario_harness_asan",
        repo_root / "cpp_harness" / "integration" / "scenario_harness.cpp",
        extra_flags=ASAN_COMPILE_FLAGS,
    )
    config_path = write_config(
        tmp_path / "asan.config",
        {"FLYTEAM1_INIT_HP": 1, "FLYTEAM2_INIT_HP": 1},
    )
    result = run_binary(executable, "stop", config_path, env=asan_runtime_env())
    record_memory_run("configuration-program", result)
    assert_clean_asan(result)
