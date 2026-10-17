"""C++ compilation helpers used by Pytest fixtures and structural probes."""

from __future__ import annotations

import os
from collections.abc import Sequence
from pathlib import Path

from framework.executor import run_command
from framework.models import CompileResult, RunResult


class CppCompiler:
    """Build deterministic compiler commands without invoking a shell."""

    def __init__(self, executable: str | None = None, standard: str = "c++11"):
        self.executable = executable or os.environ.get("CXX", "g++")
        self.standard = standard

    def version(self, timeout: float = 10.0) -> RunResult:
        return run_command([self.executable, "--version"], timeout=timeout)

    def compile(
        self,
        *,
        sources: Sequence[Path],
        output_path: Path,
        include_dirs: Sequence[Path] = (),
        extra_flags: Sequence[str] = (),
        cwd: Path | None = None,
        timeout: float = 60.0,
    ) -> CompileResult:
        output_path.parent.mkdir(parents=True, exist_ok=True)
        command: list[str | Path] = [
            self.executable,
            f"-std={self.standard}",
            *extra_flags,
            *sources,
        ]
        for include_dir in include_dirs:
            command.extend(["-I", include_dir])
        command.extend(["-o", output_path])

        observed = run_command(command, cwd=cwd, timeout=timeout)
        return CompileResult(
            command=observed.command,
            returncode=observed.returncode,
            stdout=observed.stdout,
            stderr=observed.stderr,
            duration_seconds=observed.duration_seconds,
            output_path=output_path,
            timed_out=observed.timed_out,
        )

