from __future__ import annotations

import sys

import pytest

from framework.executor import run_command


pytestmark = pytest.mark.framework


def test_executor_captures_stdout_stderr_and_exit_code() -> None:
    result = run_command(
        [
            sys.executable,
            "-c",
            "import sys; print('OUT'); print('ERR', file=sys.stderr); sys.exit(3)",
        ]
    )

    assert result.returncode == 3
    assert result.stdout == "OUT\n"
    assert result.stderr == "ERR\n"
    assert result.succeeded is False


def test_executor_reports_timeout_separately() -> None:
    result = run_command(
        [sys.executable, "-c", "import time; time.sleep(1)"],
        timeout=0.05,
    )

    assert result.timed_out is True
    assert result.returncode is None
    assert result.succeeded is False


def test_executor_reports_missing_program() -> None:
    result = run_command(["definitely-not-a-real-command-dragon-warriors"])

    assert result.returncode == 127
    assert result.succeeded is False

