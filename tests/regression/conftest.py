from __future__ import annotations

from collections.abc import Callable
from pathlib import Path

import pytest


@pytest.fixture(scope="session")
def domain_executable(
    repo_root: Path,
    compile_harness: Callable[..., Path],
) -> Path:
    return compile_harness(
        "domain_harness",
        repo_root / "cpp_harness" / "unit" / "domain_harness.cpp",
    )
