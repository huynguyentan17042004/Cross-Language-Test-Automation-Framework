from __future__ import annotations

from pathlib import Path

import pytest

from framework.compiler import CppCompiler
from framework.models import CompileResult
from framework.probes import ProbeClassification, classify_compile_result


pytestmark = pytest.mark.structural


def compile_probe(
    tmp_path: Path,
    cpp_compiler: CppCompiler,
    sut_dir: Path,
    statement: str,
) -> CompileResult:
    source = tmp_path / "probe.cpp"
    source.write_text(
        '#include "dragon.h"\n'
        "int main() {\n"
        "    Position position(1, 2);\n"
        f"    {statement}\n"
        "    return 0;\n"
        "}\n"
    )
    return cpp_compiler.compile(
        sources=[source, sut_dir / "dragon.cpp"],
        output_path=tmp_path / "probe",
        include_dirs=[sut_dir],
    )


def test_position_get_row_is_public(
    tmp_path: Path,
    cpp_compiler: CppCompiler,
    sut_dir: Path,
) -> None:
    result = compile_probe(tmp_path, cpp_compiler, sut_dir, "position.getRow();")
    assert classify_compile_result(result) is ProbeClassification.PUBLIC, result.stderr


def test_position_row_field_is_private(
    tmp_path: Path,
    cpp_compiler: CppCompiler,
    sut_dir: Path,
) -> None:
    result = compile_probe(tmp_path, cpp_compiler, sut_dir, "position.r_;")
    assert classify_compile_result(result) is ProbeClassification.PRIVATE, result.stderr

