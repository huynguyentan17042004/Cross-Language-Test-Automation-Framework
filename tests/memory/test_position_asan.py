from __future__ import annotations

from collections.abc import Callable
from pathlib import Path

import pytest

from framework.assertions import require_run_success
from framework.models import RunResult
from framework.sanitizers import ASAN_COMPILE_FLAGS, asan_runtime_env


pytestmark = [pytest.mark.memory, pytest.mark.slow]


def test_position_harness_under_address_sanitizer(
    repo_root: Path,
    compile_harness: Callable[..., Path],
    run_binary: Callable[..., RunResult],
    record_memory_run: Callable[[str, RunResult], None],
) -> None:
    executable = compile_harness(
        "position_harness_asan",
        repo_root / "cpp_harness" / "unit" / "position_harness.cpp",
        extra_flags=ASAN_COMPILE_FLAGS,
    )
    result = run_binary(
        executable,
        "construct-values",
        1,
        15,
        env=asan_runtime_env(),
    )
    record_memory_run("position", result)

    require_run_success(result)
    assert result.stdout == "(1,15)"
    assert "AddressSanitizer" not in result.stderr
