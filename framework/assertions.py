"""Readable assertion messages for external build and run failures."""

from __future__ import annotations

import shlex

from framework.models import CompileResult, RunResult


def require_compile_success(result: CompileResult) -> None:
    if result.succeeded:
        return
    raise AssertionError(
        "C++ compilation failed\n"
        f"command: {_format_command(result.command)}\n"
        f"return code: {result.returncode}\n"
        f"timed out: {result.timed_out}\n"
        f"stdout:\n{result.stdout}\n"
        f"stderr:\n{result.stderr}"
    )


def require_run_success(result: RunResult) -> None:
    if result.succeeded:
        return
    raise AssertionError(
        "Executable run failed\n"
        f"command: {_format_command(result.command)}\n"
        f"return code: {result.returncode}\n"
        f"timed out: {result.timed_out}\n"
        f"stdout:\n{result.stdout}\n"
        f"stderr:\n{result.stderr}"
    )


def _format_command(command: tuple[str, ...]) -> str:
    return " ".join(shlex.quote(part) for part in command)

