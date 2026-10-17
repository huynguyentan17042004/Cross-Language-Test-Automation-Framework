from __future__ import annotations

from pathlib import Path

import pytest

from framework.assertions import require_compile_success, require_run_success
from framework.compiler import CppCompiler
from framework.executor import run_command


pytestmark = [pytest.mark.integration, pytest.mark.slow]


def test_legacy_cases_19_to_22_execute(
    cpp_compiler: CppCompiler,
    sut_dir: Path,
    build_dir: Path,
) -> None:
    executable = build_dir / "legacy_baseline"
    compile_result = cpp_compiler.compile(
        sources=[sut_dir / "main.cpp", sut_dir / "dragon.cpp"],
        output_path=executable,
        include_dirs=[sut_dir],
        extra_flags=["-g", "-O0"],
    )
    require_compile_success(compile_result)

    run_result = run_command([executable], cwd=sut_dir, timeout=20)
    require_run_success(run_result)

    for case_id in (19, 20, 21, 22):
        assert f"----- Sample Testcase {case_id} -----" in run_result.stdout
    assert "FlyTeam1 defeated the DragonLord!" in run_result.stdout
    assert run_result.stderr == ""

