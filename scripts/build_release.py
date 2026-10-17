#!/usr/bin/env python3
"""Build and reproducibly package the Dragon Warriors console executable."""

from __future__ import annotations

import argparse
import gzip
import hashlib
import json
import os
import platform
import re
import shutil
import subprocess
import tarfile
from pathlib import Path


VERSION_PATTERN = re.compile(r"^[0-9A-Za-z][0-9A-Za-z._-]*$")
DEFAULT_EPOCH = 1_700_000_000


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    parser.add_argument("--version", required=True)
    parser.add_argument("--test-report", type=Path, required=True)
    parser.add_argument("--output-dir", type=Path, default=Path("dist"))
    return parser.parse_args()


def normalized_version(raw: str) -> str:
    version = raw.removeprefix("v")
    if not VERSION_PATTERN.fullmatch(version):
        raise ValueError(f"Invalid release version: {raw!r}")
    return version


def target_platform() -> str:
    system = platform.system().lower()
    machine = platform.machine().lower()
    if machine in {"amd64", "x86_64"}:
        machine = "x86_64"
    elif machine in {"aarch64", "arm64"}:
        machine = "arm64"
    return f"{system}-{machine}"


def repository_commit(repo_root: Path) -> str:
    configured = os.environ.get("GITHUB_SHA")
    if configured:
        return configured
    observed = subprocess.run(
        ["git", "rev-parse", "HEAD"],
        cwd=repo_root,
        check=True,
        capture_output=True,
        text=True,
    )
    return observed.stdout.strip()


def normalized_tar_info(info: tarfile.TarInfo, epoch: int) -> tarfile.TarInfo:
    info.uid = 0
    info.gid = 0
    info.uname = "root"
    info.gname = "root"
    info.mtime = epoch
    return info


def create_archive(stage: Path, archive: Path, epoch: int) -> None:
    with archive.open("wb") as raw:
        with gzip.GzipFile(fileobj=raw, mode="wb", mtime=epoch) as compressed:
            with tarfile.open(fileobj=compressed, mode="w") as bundle:
                bundle.add(
                    stage,
                    arcname=stage.name,
                    filter=lambda info: normalized_tar_info(info, epoch),
                )


def main() -> int:
    args = parse_args()
    version = normalized_version(args.version)
    repo_root = Path(__file__).resolve().parents[1]
    report = args.test_report.resolve()
    if not report.is_file():
        raise FileNotFoundError(f"JUnit report does not exist: {report}")

    output_dir = args.output_dir.resolve()
    output_dir.mkdir(parents=True, exist_ok=True)
    package_name = f"dragon-warriors-{version}-{target_platform()}"
    stage = repo_root / "build" / "release-stage" / package_name
    if stage.exists():
        shutil.rmtree(stage)
    (stage / "config").mkdir(parents=True)

    compiler = os.environ.get("CXX", "g++")
    binary = stage / "dragon-warriors"
    command = [
        compiler,
        "-std=c++11",
        "-O2",
        "sut/legacy/main.cpp",
        "sut/legacy/dragon.cpp",
        "-I",
        "sut/legacy",
        "-o",
        str(binary),
    ]
    subprocess.run(command, cwd=repo_root, check=True)
    binary.chmod(0o755)

    for config in sorted((repo_root / "sut" / "legacy" / "config").iterdir()):
        if config.is_file():
            shutil.copy2(config, stage / "config" / config.name)
    shutil.copy2(repo_root / "release" / "README.md", stage / "README.md")
    shutil.copy2(report, stage / "test-report.xml")

    manifest = {
        "version": version,
        "commit": repository_commit(repo_root),
        "platform": target_platform(),
        "compiler": compiler,
        "compile_command": [*command[:-1], "dragon-warriors"],
        "test_report": "test-report.xml",
    }
    (stage / "manifest.json").write_text(
        json.dumps(manifest, indent=2, sort_keys=True) + "\n",
        encoding="utf-8",
    )

    epoch = int(os.environ.get("SOURCE_DATE_EPOCH", DEFAULT_EPOCH))
    archive = output_dir / f"{package_name}.tar.gz"
    create_archive(stage, archive, epoch)
    digest = hashlib.sha256(archive.read_bytes()).hexdigest()
    checksum = archive.with_suffix(archive.suffix + ".sha256")
    checksum.write_text(f"{digest}  {archive.name}\n", encoding="utf-8")
    print(archive)
    print(checksum)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
