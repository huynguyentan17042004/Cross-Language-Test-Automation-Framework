from __future__ import annotations

from pathlib import Path

import pytest

from framework.compiler import CppCompiler


pytestmark = pytest.mark.framework


def test_compiler_builds_valid_cpp(tmp_path: Path, cpp_compiler: CppCompiler) -> None:
    source = tmp_path / "valid.cpp"
    source.write_text("int main() { return 0; }\n")
    output = tmp_path / "valid"

    result = cpp_compiler.compile(sources=[source], output_path=output)

    assert result.succeeded, result.stderr
    assert output.is_file()


def test_compiler_preserves_diagnostics(
    tmp_path: Path, cpp_compiler: CppCompiler
) -> None:
    source = tmp_path / "invalid.cpp"
    source.write_text("int main( { return 0; }\n")
    output = tmp_path / "invalid"

    result = cpp_compiler.compile(sources=[source], output_path=output)

    assert result.succeeded is False
    assert result.returncode != 0
    assert result.stderr
    assert output.exists() is False

