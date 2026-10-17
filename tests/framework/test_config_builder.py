from __future__ import annotations

from pathlib import Path

import pytest

from framework.configuration import DEFAULT_CONFIG, write_config


pytestmark = pytest.mark.framework


def test_write_config_preserves_complete_key_order(tmp_path: Path) -> None:
    path = write_config(tmp_path / "sample.config")

    keys = [line.split("=", 1)[0] for line in path.read_text().splitlines()]
    assert keys == list(DEFAULT_CONFIG)


def test_write_config_rejects_unknown_key(tmp_path: Path) -> None:
    with pytest.raises(ValueError, match="UNKNOWN_KEY"):
        write_config(tmp_path / "invalid.config", {"UNKNOWN_KEY": 1})
