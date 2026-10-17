"""Result models shared by compiler, executor, probes, and tests."""

from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path


@dataclass(frozen=True)
class RunResult:
    """Observed result from one external process."""

    command: tuple[str, ...]
    returncode: int | None
    stdout: str
    stderr: str
    duration_seconds: float
    timed_out: bool = False

    @property
    def succeeded(self) -> bool:
        return not self.timed_out and self.returncode == 0


@dataclass(frozen=True)
class CompileResult:
    """Observed result from one C++ compilation."""

    command: tuple[str, ...]
    returncode: int | None
    stdout: str
    stderr: str
    duration_seconds: float
    output_path: Path
    timed_out: bool = False

    @property
    def succeeded(self) -> bool:
        return (
            not self.timed_out
            and self.returncode == 0
            and self.output_path.is_file()
        )


@dataclass(frozen=True)
class ProbeResult:
    """Classification produced by a compile-time OOP probe."""

    contract_id: str
    expected: str
    actual: str
    diagnostic: str

    @property
    def passed(self) -> bool:
        return self.expected == self.actual

