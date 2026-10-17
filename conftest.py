"""Shared Pytest fixtures for compiling and executing C++ harnesses."""

from __future__ import annotations

from collections.abc import Callable, Sequence
from pathlib import Path

import pytest

from framework.assertions import require_compile_success
from framework.compiler import CppCompiler
from framework.executor import run_command
from framework.models import RunResult


pytest_plugins = ["framework.pytest_plugin"]


@pytest.fixture(scope="session")
def repo_root() -> Path:
    return Path(__file__).resolve().parent


@pytest.fixture(scope="session")
def sut_dir(repo_root: Path) -> Path:
    return repo_root / "sut" / "legacy"


@pytest.fixture(scope="session")
def cpp_compiler() -> CppCompiler:
    compiler = CppCompiler()
    version = compiler.version()
    if not version.succeeded:
        pytest.fail(f"C++ compiler unavailable:\n{version.stderr}")
    return compiler


@pytest.fixture(scope="session")
def build_dir(tmp_path_factory: pytest.TempPathFactory) -> Path:
    return tmp_path_factory.mktemp("cpp-build")


@pytest.fixture(scope="session")
def compile_harness(
    cpp_compiler: CppCompiler,
    build_dir: Path,
    sut_dir: Path,
) -> Callable[..., Path]:
    cache: dict[tuple[object, ...], Path] = {}

    def compile_one(
        name: str,
        harness: Path,
        *,
        extra_sources: Sequence[Path] = (),
        extra_flags: Sequence[str] = (),
    ) -> Path:
        key = (name, harness, tuple(extra_sources), tuple(extra_flags))
        if key in cache:
            return cache[key]

        output_path = build_dir / name
        result = cpp_compiler.compile(
            sources=[harness, sut_dir / "dragon.cpp", *extra_sources],
            output_path=output_path,
            include_dirs=[sut_dir],
            extra_flags=extra_flags,
        )
        require_compile_success(result)
        cache[key] = output_path
        return output_path

    return compile_one


@pytest.fixture
def run_binary() -> Callable[..., RunResult]:
    def run_one(
        executable: Path,
        *args: object,
        cwd: Path | None = None,
        timeout: float = 10.0,
    ) -> RunResult:
        return run_command(
            [executable, *(str(arg) for arg in args)],
            cwd=cwd,
            timeout=timeout,
        )

    return run_one
