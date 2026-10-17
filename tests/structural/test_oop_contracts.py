from __future__ import annotations

import shlex
from dataclasses import dataclass
from pathlib import Path

import pytest

from framework.compiler import CppCompiler
from framework.probes import (
    ProbeClassification,
    compile_cpp_probe,
    make_probe_result,
)


pytestmark = pytest.mark.structural


@dataclass(frozen=True)
class ProbeCase:
    contract_id: str
    source_body: str
    expected: ProbeClassification


PROBE_CASES = (
    ProbeCase(
        "position-get-row-public",
        "    Position position(1, 2);\n    (void)position.getRow();",
        ProbeClassification.COMPILES,
    ),
    ProbeCase(
        "position-get-col-public",
        "    Position position(1, 2);\n    (void)position.getCol();",
        ProbeClassification.COMPILES,
    ),
    ProbeCase(
        "position-row-private",
        "    Position position(1, 2);\n    (void)position.r_;",
        ProbeClassification.PRIVATE,
    ),
    ProbeCase(
        "position-column-private",
        "    Position position(1, 2);\n    (void)position.c_;",
        ProbeClassification.PRIVATE,
    ),
    ProbeCase(
        "map-element-required-damage-private",
        "    MapElement element(PATH);\n    (void)element.req_dmg;",
        ProbeClassification.PRIVATE,
    ),
    ProbeCase(
        "moving-object-index-protected",
        "    MovingObject *object = nullptr;\n    (void)object->index;",
        ProbeClassification.PROTECTED,
    ),
    ProbeCase(
        "fly-team-inherits-warrior",
        '    static_assert(std::is_base_of<Warrior, FlyTeam>::value, "FlyTeam must inherit Warrior");',
        ProbeClassification.COMPILES,
    ),
    ProbeCase(
        "ground-team-inherits-warrior",
        '    static_assert(std::is_base_of<Warrior, GroundTeam>::value, "GroundTeam must inherit Warrior");',
        ProbeClassification.COMPILES,
    ),
    ProbeCase(
        "moving-object-is-abstract",
        '    static_assert(std::is_abstract<MovingObject>::value, "MovingObject must be abstract");',
        ProbeClassification.COMPILES,
    ),
    ProbeCase(
        "moving-object-has-virtual-destructor",
        '    static_assert(std::has_virtual_destructor<MovingObject>::value, "MovingObject destructor must be virtual");',
        ProbeClassification.COMPILES,
    ),
)


@pytest.fixture(scope="session")
def structural_diagnostics_dir(repo_root: Path) -> Path:
    output = repo_root / "reports" / "structural-diagnostics"
    output.mkdir(parents=True, exist_ok=True)
    return output


@pytest.mark.parametrize("case", PROBE_CASES, ids=lambda case: case.contract_id)
def test_oop_contract_probe(
    case: ProbeCase,
    tmp_path: Path,
    cpp_compiler: CppCompiler,
    sut_dir: Path,
    structural_diagnostics_dir: Path,
) -> None:
    compile_result = compile_cpp_probe(
        compiler=cpp_compiler,
        sut_dir=sut_dir,
        work_dir=tmp_path,
        contract_id=case.contract_id,
        source_body=case.source_body,
    )
    probe_result = make_probe_result(
        case.contract_id,
        case.expected,
        compile_result,
    )

    diagnostic_path = structural_diagnostics_dir / f"{case.contract_id}.log"
    diagnostic_path.write_text(
        f"contract: {case.contract_id}\n"
        f"expected: {probe_result.expected}\n"
        f"actual: {probe_result.actual}\n"
        f"returncode: {compile_result.returncode}\n"
        f"timed_out: {compile_result.timed_out}\n"
        f"command: {shlex.join(compile_result.command)}\n"
        f"stdout:\n{compile_result.stdout}\n"
        f"stderr:\n{compile_result.stderr}\n",
        encoding="utf-8",
    )

    assert probe_result.passed, (
        f"contract {case.contract_id}: expected {probe_result.expected}, "
        f"observed {probe_result.actual}\n{probe_result.diagnostic}"
    )
