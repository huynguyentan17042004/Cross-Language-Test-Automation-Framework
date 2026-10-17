from __future__ import annotations

from pathlib import Path

import pytest

from framework.models import CompileResult
from framework.probes import ProbeClassification, classify_compile_result


pytestmark = pytest.mark.framework


def compile_result(
    tmp_path: Path,
    *,
    returncode: int | None,
    stderr: str = "",
    timed_out: bool = False,
    create_output: bool = False,
) -> CompileResult:
    output = tmp_path / "probe"
    if create_output:
        output.touch()
    return CompileResult(
        command=("g++", "probe.cpp"),
        returncode=returncode,
        stdout="",
        stderr=stderr,
        duration_seconds=0.01,
        output_path=output,
        timed_out=timed_out,
    )


def test_successful_probe_is_public(tmp_path: Path) -> None:
    result = compile_result(tmp_path, returncode=0, create_output=True)
    assert classify_compile_result(result) is ProbeClassification.PUBLIC


@pytest.mark.parametrize(
    ("diagnostic", "expected"),
    [
        (
            "error: 'r_' is a private member of 'Position'",
            ProbeClassification.PRIVATE,
        ),
        (
            "error: 'hp' is a protected member of 'Warrior'",
            ProbeClassification.PROTECTED,
        ),
        (
            "error: no member named 'missing' in 'Position'",
            ProbeClassification.CONTRACT_MISMATCH,
        ),
    ],
    ids=("private-member", "protected-member", "contract-mismatch"),
)
def test_probe_diagnostic_classification(
    tmp_path: Path,
    diagnostic: str,
    expected: ProbeClassification,
) -> None:
    result = compile_result(tmp_path, returncode=1, stderr=diagnostic)
    assert classify_compile_result(result) is expected


def test_probe_timeout_is_toolchain_error(tmp_path: Path) -> None:
    result = compile_result(tmp_path, returncode=None, timed_out=True)
    assert classify_compile_result(result) is ProbeClassification.TOOLCHAIN_ERROR
