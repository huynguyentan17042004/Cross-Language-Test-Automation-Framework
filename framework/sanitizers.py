"""Compiler and runtime settings for AddressSanitizer test runs."""

from __future__ import annotations

import platform

ASAN_COMPILE_FLAGS = (
    "-g",
    "-O1",
    "-fsanitize=address",
    "-fno-omit-frame-pointer",
)


def asan_runtime_env() -> dict[str, str]:
    """Enable leak detection only where the ASan runtime supports it."""

    options = ["halt_on_error=1"]
    if platform.system() == "Linux":
        options.append("detect_leaks=1")
    return {"ASAN_OPTIONS": ":".join(options)}
