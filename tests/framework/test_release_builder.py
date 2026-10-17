from __future__ import annotations

import importlib.util
from pathlib import Path

import pytest


pytestmark = pytest.mark.framework


def load_release_builder(repo_root: Path):
    path = repo_root / "scripts" / "build_release.py"
    spec = importlib.util.spec_from_file_location("build_release", path)
    assert spec is not None and spec.loader is not None
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


@pytest.mark.parametrize(
    ("raw", "expected"),
    [("1.0.0", "1.0.0"), ("v2.1.0-rc1", "2.1.0-rc1")],
)
def test_release_version_normalization(
    repo_root: Path,
    raw: str,
    expected: str,
) -> None:
    builder = load_release_builder(repo_root)
    assert builder.normalized_version(raw) == expected


def test_release_version_rejects_shell_metacharacters(repo_root: Path) -> None:
    builder = load_release_builder(repo_root)
    with pytest.raises(ValueError):
        builder.normalized_version("1.0.0;echo-danger")


def test_release_target_platform_has_no_spaces(repo_root: Path) -> None:
    builder = load_release_builder(repo_root)
    assert " " not in builder.target_platform()
    assert "-" in builder.target_platform()
