from __future__ import annotations

from pathlib import Path

import pytest

from framework.diagnostics import write_run_diagnostics
from framework.models import RunResult


pytestmark = pytest.mark.framework


def test_write_run_diagnostics_preserves_process_evidence(tmp_path: Path) -> None:
    result = RunResult(
        command=("program", "argument"),
        returncode=3,
        stdout="observed output\n",
        stderr="observed error\n",
        duration_seconds=0.125,
        timed_out=False,
    )

    write_run_diagnostics(tmp_path, "sample", result)

    assert (tmp_path / "sample.stdout.txt").read_text() == "observed output\n"
    assert (tmp_path / "sample.stderr.txt").read_text() == "observed error\n"
    metadata = (tmp_path / "sample.run.txt").read_text()
    assert "command=program argument" in metadata
    assert "returncode=3" in metadata
    assert "duration_seconds=0.125000" in metadata
