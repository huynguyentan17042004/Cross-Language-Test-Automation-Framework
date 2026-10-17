"""Readable layer selection and tagged terminal output for the test suite."""

from __future__ import annotations

from collections import Counter, defaultdict
from typing import Any


TAG_ORDER = (
    "framework",
    "smoke",
    "unit",
    "structural",
    "integration",
    "memory",
    "slow",
)


def pytest_addoption(parser: Any) -> None:
    group = parser.getgroup("dragon-warriors")
    group.addoption(
        "--layer",
        action="store",
        choices=("all", *TAG_ORDER),
        default="all",
        help="Run one Dragon Warriors test layer/tag (default: all).",
    )


def pytest_report_header(config: Any) -> list[str]:
    selected = config.getoption("--layer")
    return [
        f"Dragon Warriors layer: {selected}",
        "Visible tags: " + ", ".join(tag.upper() for tag in TAG_ORDER),
    ]


def pytest_collection_modifyitems(config: Any, items: list[Any]) -> None:
    selected = config.getoption("--layer")
    kept: list[Any] = []
    deselected: list[Any] = []
    tag_counts: Counter[str] = Counter()

    for item in items:
        tags = tags_from_item(item)
        item.user_properties.append(("test_tags", ",".join(tags)))
        if selected != "all" and selected not in tags:
            deselected.append(item)
            continue
        kept.append(item)
        tag_counts.update(tags)

    if deselected:
        config.hook.pytest_deselected(items=deselected)
        items[:] = kept

    config._dragon_warriors_tag_counts = tag_counts


def pytest_report_teststatus(report: Any, config: Any) -> tuple[str, str, str] | None:
    del config
    if getattr(report, "wasxfail", False):
        return None

    label = format_report_tags(report)
    if report.when == "call":
        if report.passed:
            return "passed", ".", f"PASSED {label}"
        if report.failed:
            return "failed", "F", f"FAILED {label}"
        if report.skipped:
            return "skipped", "s", f"SKIPPED {label}"
    if report.when == "setup" and report.failed:
        return "error", "E", f"ERROR {label}"
    return None


def pytest_terminal_summary(terminalreporter: Any, exitstatus: int, config: Any) -> None:
    del exitstatus
    collected: Counter[str] = getattr(
        config, "_dragon_warriors_tag_counts", Counter()
    )
    outcomes: dict[str, Counter[str]] = defaultdict(Counter)

    for outcome in ("passed", "failed", "skipped", "error"):
        for report in terminalreporter.stats.get(outcome, []):
            if report.when != "call" and not (
                outcome == "error" and report.when == "setup"
            ):
                continue
            for tag in tags_from_report(report):
                outcomes[tag][outcome] += 1

    terminalreporter.section("Dragon Warriors tag summary")
    for tag in TAG_ORDER:
        if not collected[tag]:
            continue
        counts = outcomes[tag]
        terminalreporter.write_line(
            f"{tag.upper():<12} selected={collected[tag]:<3} "
            f"passed={counts['passed']:<3} failed={counts['failed']:<3} "
            f"skipped={counts['skipped']:<3} errors={counts['error']:<3}"
        )


def tags_from_item(item: Any) -> tuple[str, ...]:
    return tuple(tag for tag in TAG_ORDER if item.get_closest_marker(tag) is not None)


def tags_from_report(report: Any) -> tuple[str, ...]:
    for key, value in getattr(report, "user_properties", []):
        if key == "test_tags" and value:
            return tuple(value.split(","))
    return ()


def format_report_tags(report: Any) -> str:
    tags = tags_from_report(report)
    return "[" + ", ".join(tag.upper() for tag in tags) + "]"

