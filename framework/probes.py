"""Classification rules for compile-time OOP contract probes."""

from __future__ import annotations

from enum import StrEnum
from pathlib import Path

from framework.compiler import CppCompiler
from framework.models import CompileResult, ProbeResult


class ProbeClassification(StrEnum):
    COMPILES = "compiles"
    PRIVATE = "private"
    PROTECTED = "protected"
    CONTRACT_MISMATCH = "contract_mismatch"
    TOOLCHAIN_ERROR = "toolchain_error"
    UNKNOWN = "unknown"


def compile_cpp_probe(
    *,
    compiler: CppCompiler,
    sut_dir: Path,
    work_dir: Path,
    contract_id: str,
    source_body: str,
    timeout: float = 60.0,
) -> CompileResult:
    """Compile one isolated OOP contract probe against the C++ SUT."""

    source = work_dir / f"{contract_id}.cpp"
    source.write_text(
        '#include "dragon.h"\n'
        "#include <type_traits>\n"
        "int main() {\n"
        f"{source_body}\n"
        "    return 0;\n"
        "}\n",
        encoding="utf-8",
    )
    return compiler.compile(
        sources=[source, sut_dir / "dragon.cpp"],
        output_path=work_dir / contract_id,
        include_dirs=[sut_dir],
        timeout=timeout,
    )


def classify_compile_result(result: CompileResult) -> ProbeClassification:
    """Classify why a probe compiled or failed; never treat every failure as pass."""

    if result.succeeded:
        return ProbeClassification.COMPILES
    if result.timed_out or result.returncode == 127:
        return ProbeClassification.TOOLCHAIN_ERROR

    diagnostic = result.stderr.lower()
    if _contains_any(
        diagnostic,
        ("is a private member", "is private within this context", "private field"),
    ):
        return ProbeClassification.PRIVATE
    if _contains_any(
        diagnostic,
        (
            "is a protected member",
            "is protected within this context",
            "protected field",
        ),
    ):
        return ProbeClassification.PROTECTED
    if _contains_any(
        diagnostic,
        (
            "no member named",
            "has no member named",
            "has no member",
            "no matching function",
            "no matching constructor",
            "too many arguments",
            "candidate function not viable",
            "use of undeclared identifier",
        ),
    ):
        return ProbeClassification.CONTRACT_MISMATCH
    if _contains_any(
        diagnostic,
        (
            "file not found",
            "unable to execute command",
            "cannot find -l",
            "no such file or directory",
        ),
    ):
        return ProbeClassification.TOOLCHAIN_ERROR
    return ProbeClassification.UNKNOWN


def make_probe_result(
    contract_id: str,
    expected: ProbeClassification,
    compile_result: CompileResult,
) -> ProbeResult:
    actual = classify_compile_result(compile_result)
    return ProbeResult(
        contract_id=contract_id,
        expected=expected.value,
        actual=actual.value,
        diagnostic=compile_result.stderr,
    )


def _contains_any(text: str, candidates: tuple[str, ...]) -> bool:
    return any(candidate in text for candidate in candidates)
