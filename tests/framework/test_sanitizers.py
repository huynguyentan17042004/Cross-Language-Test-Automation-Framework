from __future__ import annotations

import pytest

from framework.sanitizers import asan_runtime_env


pytestmark = pytest.mark.framework


def test_asan_runtime_always_stops_on_first_error() -> None:
    assert "halt_on_error=1" in asan_runtime_env()["ASAN_OPTIONS"]
