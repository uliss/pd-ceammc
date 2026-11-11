#!/usr/bin/env python3
"""
Check <see> tags in pddoc documentation files.

Collects all object names from <object name="..."> plus vanilla Pd object names
from core_objects.txt, and verifies that every <see>...</see> references an
existing object.

Usage:
    python check_see.py /path/to/docs
    python check_see.py /path/to/docs --ignore ceammc/ ui.
"""

import argparse
import sys
import xml.etree.ElementTree as ET
from pathlib import Path


# --- ANSI color helpers -----------------------------------------------------

class Color:
    RESET = "\033[0m"
    BOLD = "\033[1m"
    RED = "\033[31m"
    GREEN = "\033[32m"
    YELLOW = "\033[33m"
    BLUE = "\033[34m"
    CYAN = "\033[36m"
    GRAY = "\033[90m"


def supports_color() -> bool:
    """Return True if stdout supports ANSI colors."""
    return sys.stdout.isatty()


# Disable colors when piped or redirected
if not supports_color():
    for attr in ("RESET", "BOLD", "RED", "GREEN", "YELLOW", "BLUE", "CYAN", "GRAY"):
        setattr(Color, attr, "")


def c(text: str, color: str) -> str:
    """Wrap text in a color, if colors are enabled."""
    return f"{color}{text}{Color.RESET}"


# --- Core logic -------------------------------------------------------------

DEFAULT_CORE_OBJECTS = "@PROJECT_SOURCE_DIR@/ceammc/ext/core_objects.txt"


def find_files(folder: Path) -> list[Path]:
    """Recursively find all *.pddoc files in the folder."""
    return sorted(folder.rglob("*pddoc"))


def load_core_objects(path: Path) -> set[str]:
    """
    Load vanilla Pd object names, one per line.
    Empty lines and lines starting with '#' are ignored.
    """
    if not path.is_file():
        print(
            c(f"Warning: core objects file not found: {path}", Color.YELLOW),
            file=sys.stderr,
        )
        return set()

    names: set[str] = set()
    for line in path.read_text(encoding="utf-8").splitlines():
        line = line.strip()
        if not line or line.startswith("#"):
            continue
        names.add(line)
    return names


def extract_object_name(xml_path: Path) -> tuple[str | None, str | None]:
    """
    Return (object_name, error_message).
    object_name is None if parsing failed or <object name=...> is missing.
    """
    try:
        tree = ET.parse(xml_path)
    except ET.ParseError as e:
        return None, f"XML parse error: {e}"

    root = tree.getroot()
    # The root element is usually <pddoc>, with <object name="..."> inside
    obj = root.find("object")
    if obj is None:
        return None, "no <object> element"
    name = obj.get("name")
    if not name:
        return None, "<object> has no name attribute"
    return name, None


def extract_see_refs(xml_path: Path) -> list[str]:
    """
    Return a list of <see>...</see> texts.
    """
    refs: list[str] = []
    try:
        tree = ET.parse(xml_path)
    except ET.ParseError:
        return refs  # parse error already handled in extract_object_name

    for see in tree.getroot().iter("see"):
        text = (see.text or "").strip()
        if text:
            refs.append(text)
    return refs


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Check <see> tags for valid object names in pddoc files."
    )
    parser.add_argument(
        "folder",
        nargs="?",
        default="@PROJECT_SOURCE_DIR@/ceammc/ext/doc",
        type=Path,
        help="folder with documentation files",
    )
    parser.add_argument(
        "--core",
        type=Path,
        default=DEFAULT_CORE_OBJECTS,
        help="path to core_objects.txt (one vanilla Pd object per line)",
    )
    parser.add_argument(
        "--ignore",
        nargs="*",
        default=[],
        help="name prefixes to ignore when checking <see>",
    )
    parser.add_argument(
        "-v", "--verbose",
        action="store_true",
        help="print every checked reference, not just the errors",
    )

    args = parser.parse_args()

    if not args.folder.is_dir():
        print(c(f"Error: '{args.folder}' is not a directory", Color.RED), file=sys.stderr)
        return 1

    files = find_files(args.folder)
    if not files:
        print(c(f"No pddoc files found in '{args.folder}'", Color.RED), file=sys.stderr)
        return 1

    # Load vanilla Pd object names
    core_objects = load_core_objects(args.core)

    hardcoded_objects = set(["fiddle~", "sigmund~", "hilbert~"])

    # 1. Collect all object names
    object_names: set[str] = set()
    parse_errors: list[tuple[Path, str]] = []

    for f in files:
        name, err = extract_object_name(f)
        if err:
            parse_errors.append((f, err))
            continue
        object_names.add(name)

    # Vanilla Pd objects also count as valid <see> targets
    known_names = object_names | core_objects | hardcoded_objects

    print(c("Files found:", Color.BOLD), len(files))
    print(c("Unique object names:", Color.BOLD), len(object_names))
    print(c("Vanilla Pd objects:", Color.BOLD), len(core_objects))
    if parse_errors:
        print(c(f"Files with parse errors: {len(parse_errors)}", Color.YELLOW))
        for f, err in parse_errors:
            print(f"  {c('[parse]', Color.YELLOW)} {f}: {err}")

    # 2. Check <see>
    total_refs = 0
    bad_refs: list[tuple[Path, str]] = []
    ignored = 0

    for f in files:
        for ref in extract_see_refs(f):
            total_refs += 1

            if any(ref.startswith(p) for p in args.ignore):
                ignored += 1
                if args.verbose:
                    print(f"  {c('[skip]', Color.GRAY)} {f}: <see>{ref}</see>")
                continue

            # Handle possible forms: "ceammc/xxx", "xxx~", "ui.hsl", etc.
            # Main candidate is the whole string and also the part after '/'.
            candidates = {ref}
            if "/" in ref:
                candidates.add(ref.split("/", 1)[1])

            if not (candidates & known_names):
                bad_refs.append((f, ref))
                print(f"  {c('[bad]', Color.RED)}  {f}: <see>{ref}</see>")
            elif args.verbose:
                print(f"  {c('[ok]', Color.GREEN)}   {f}: <see>{ref}</see>")

    # 3. Summary
    print()
    print(f"{c('Total <see>:', Color.BOLD)} {total_refs}")
    if ignored:
        print(f"{c('Skipped by --ignore:', Color.BOLD)} {ignored}")
    print(f"{c('Broken references:', Color.BOLD)} {len(bad_refs)}")

    if bad_refs:
        print(c("\nBroken reference list:", Color.RED + Color.BOLD))
        for f, ref in bad_refs:
            print(f"  {f}: <see>{ref}</see>")
        return 2

    return 0


if __name__ == "__main__":
    sys.exit(main())