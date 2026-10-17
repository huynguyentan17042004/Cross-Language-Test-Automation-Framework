from __future__ import annotations

from collections.abc import Callable
from pathlib import Path

import pytest

from framework.assertions import require_run_success
from framework.models import RunResult


pytestmark = [pytest.mark.memory, pytest.mark.slow]


def test_position_harness_under_address_sanitizer(
    repo_root: Path,
    compile_harness: Callable[..., Path],
    run_binary: Callable[..., RunResult],
) -> None:
    executable = compile_harness(
        "position_harness_asan",
        repo_root / "cpp_harness" / "unit" / "position_harness.cpp",
        extra_flags=(
            "-g",
            "-O1",
            "-fsanitize=address",
            "-fno-omit-frame-pointer",
        ),
    )
    result = run_binary(executable, "construct-values", 1, 15)

    require_run_success(result)
    assert result.stdout == "(1,15)"
    assert "AddressSanitizer" not in result.stderr

