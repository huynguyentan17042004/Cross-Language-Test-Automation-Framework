"""Artifact-friendly diagnostic writers for external process runs."""

from __future__ import annotations

from pathlib import Path

from framework.models import RunResult


def write_run_diagnostics(directory: Path, name: str, result: RunResult) -> None:
    """Persist stdout, stderr, command, timing, and exit state for one run."""

    directory.mkdir(parents=True, exist_ok=True)
    stem = directory / name
    stem.with_suffix(".stdout.txt").write_text(result.stdout, encoding="utf-8")
    stem.with_suffix(".stderr.txt").write_text(result.stderr, encoding="utf-8")
    stem.with_suffix(".run.txt").write_text(
        "\n".join(
            (
                f"command={' '.join(result.command)}",
                f"returncode={result.returncode}",
                f"timed_out={result.timed_out}",
                f"duration_seconds={result.duration_seconds:.6f}",
            )
        )
        + "\n",
        encoding="utf-8",
    )
