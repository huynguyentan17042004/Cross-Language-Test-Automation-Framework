"""Safe subprocess execution with captured diagnostics and timeouts."""

from __future__ import annotations

import os
import subprocess
import time
from collections.abc import Mapping, Sequence
from pathlib import Path

from framework.models import RunResult


def run_command(
    command: Sequence[str | os.PathLike[str]],
    *,
    cwd: Path | None = None,
    timeout: float = 10.0,
    env: Mapping[str, str] | None = None,
) -> RunResult:
    """Run one process without a shell and preserve all observable evidence."""

    normalized = tuple(os.fspath(part) for part in command)
    started = time.monotonic()
    try:
        completed = subprocess.run(
            normalized,
            cwd=cwd,
            env={**os.environ, **env} if env is not None else None,
            capture_output=True,
            text=True,
            timeout=timeout,
            check=False,
        )
    except subprocess.TimeoutExpired as exc:
        return RunResult(
            command=normalized,
            returncode=None,
            stdout=_as_text(exc.stdout),
            stderr=_as_text(exc.stderr),
            duration_seconds=time.monotonic() - started,
            timed_out=True,
        )
    except FileNotFoundError as exc:
        return RunResult(
            command=normalized,
            returncode=127,
            stdout="",
            stderr=str(exc),
            duration_seconds=time.monotonic() - started,
        )

    return RunResult(
        command=normalized,
        returncode=completed.returncode,
        stdout=completed.stdout,
        stderr=completed.stderr,
        duration_seconds=time.monotonic() - started,
    )


def _as_text(value: str | bytes | None) -> str:
    if value is None:
        return ""
    if isinstance(value, bytes):
        return value.decode(errors="replace")
    return value
