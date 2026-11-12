#!/usr/bin/env python3

import argparse
import sys
from pathlib import Path

def find_untranslated(folder: Path) -> list[Path]:
    pattern = "*.pddoc"
    files = folder.glob(pattern)
    return [f for f in files if 'finished="false"' in f.read_text(encoding="utf-8")]

def main() -> int:
    parser = argparse.ArgumentParser(
        description="Проверка полноты перевода файлов документации (ищет finished=\"false\")"
    )
    parser.add_argument(
        "folder",
        nargs="?",
        default="@PROJECT_SOURCE_DIR@/ceammc/ext/doc",
        type=Path,
        help="папка с файлами документации",
    )
    parser.add_argument(
        "-l", "--list-untranslated",
        action="store_true",
        help="вывести список файлов с finished=\"false\"",
    )
    parser.add_argument(
        "-q", "--quiet",
        action="store_true",
        help="не выводить статистику, только список (с -l)",
    )

    args = parser.parse_args()

    if not args.folder.is_dir():
        print(f"Ошибка: '{args.folder}' не является папкой", file=sys.stderr)
        return 1

    pattern = "*.pddoc"
    all_files = list(args.folder.glob(pattern))
    total = len(all_files)

    if total == 0:
        print(f"В папке '{args.folder}' не найдено файлов *.pddoc", file=sys.stderr)
        return 1

    untranslated = find_untranslated(args.folder)
    translated = total - len(untranslated)
    percent = translated / total * 100

    if not args.quiet:
        print(f"Всего файлов:          {total}")
        print(f"С finished=\"false\":   {len(untranslated)}")
        print(f"Полностью переведено:  {translated}")
        print(f"Процент:               {percent:.1f}%")

    if args.list_untranslated:
        if untranslated:
            if not args.quiet:
                print()
            for f in sorted(untranslated):
                print(f)
        elif not args.quiet:
            print("\nНепереведённых файлов нет.")

    return 0


if __name__ == "__main__":
    sys.exit(main())