from __future__ import annotations

from collections.abc import Callable
from pathlib import Path

import pytest

from framework.assertions import require_run_success
from framework.models import RunResult


pytestmark = [pytest.mark.integration, pytest.mark.slow]


@pytest.fixture(scope="module")
def legacy_case_executable(
    repo_root: Path,
    compile_harness: Callable[..., Path],
) -> Path:
    return compile_harness(
        "legacy_case_harness",
        repo_root / "cpp_harness" / "integration" / "legacy_case_harness.cpp",
        extra_flags=["-g", "-O0"],
    )


@pytest.mark.parametrize("case_id", [19, 20, 21, 22])
def test_legacy_case_executes_independently(
    case_id: int,
    legacy_case_executable: Path,
    sut_dir: Path,
    run_binary: Callable[..., RunResult],
    record_integration_run: Callable[[str, RunResult], None],
) -> None:
    result = run_binary(
        legacy_case_executable,
        case_id,
        cwd=sut_dir,
        timeout=20,
    )
    record_integration_run(f"legacy-{case_id}", result)
    require_run_success(result)

    assert f"----- Sample Testcase {case_id} -----" in result.stdout
    assert "!BEGIN!" in result.stdout
    assert "!END!" in result.stdout
    assert result.stderr == ""
