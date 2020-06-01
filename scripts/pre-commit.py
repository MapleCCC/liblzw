#!/usr/bin/env python3

# This pre-commit hook script basically does three things:
# 1. Format staged C/C++ code
# 2. Transform LaTeX math equation in README.raw.md to image url in README.md
# 3. Append content of TODO.md and CHANGELOG.md to README.md

# TODO: search in google with query: "git status machine readable"

import os
import subprocess
from pathlib import Path
from subprocess import CalledProcessError
from typing import Iterable, Tuple

CLANG_FORMAT_APPLICABLE_FILE_EXTENSIONS = {
    "c",
    "cpp",
    "h",
    "tpp",
    "C",
    "CPP",
    "H",
    "TPP",
}


def get_filename_extension(filepath: str) -> str:
    return os.path.splitext(os.path.basename(filepath))[1]


def file_status_is_unmerged(index_state: str, work_tree_state: str) -> bool:
    """ Check if current git repository is in the middle of merge conflicts state """
    return (
        index_state == "U"
        or work_tree_state == "U"
        or (index_state == work_tree_state == "A")
        or (index_state == work_tree_state == "D")
    )


def get_staged_files() -> Iterable[str]:
    """ Get file paths of all staged files in current Git repository """

    completed_process = subprocess.run(
        ["git", "status", "--porcelain", "--no-renames"],
        capture_output=True,
        text=True,
        encoding="utf-8",
    )

    if completed_process.returncode != 0:
        raise RuntimeError("Error running `git status --porcerlain`")

    for line in completed_process.stdout.splitlines():
        # index_status, work_tree_status = line[:2]
        # filepath = line[2:]

        # # TODO
        # if file_status_is_unmerged(index_status, work_tree_status):
        #     # There are merge conflicts
        #     # To avoid over-complicating logic, we currently don't deal with situation of merge conflicts.
        #     raise NotImplementedError("File with merge conflicts logic not yet implemented")

        # # TODO
        # if index_status in {"R", "C"}:
        #     raise NotImplementedError("Renamed files logic not yet implemented")

        # if index_status in {"M", "A"}:
        #     # For now, we don't deal with partially staged files, very complicated.
        #     if work_tree_status in :

        #     yield filepath

        # FIXME: for simplicity, we don't handle partially staged files.
        # Future improvement is needed.
        if line[:2] in ("M ", "A "):
            yield line[3:]


def get_file_status(filepath: str):
    cmpl_proc = subprocess.run(
        ["git", "status", "--porcelain", "--no-renames", "--", filepath],
        capture_output=True,
        text=True,
        encoding="utf-8",
    )

    if cmpl_proc.returncode != 0:
        raise RuntimeError(f"Error getting status of {filepath}")

    # stdout = cmpl_proc.stdout
    # if len(stdout) >= 2:
    #     index_status, work_tree_status = cmpl_proc.stdout[:2]
    #     return index_status, work_tree_status
    # else:
    #     return " ", " "

    stdout = cmpl_proc.stdout
    if len(stdout) >= 2:
        return stdout[:2]
    else:
        return "  "


def transform_readme() -> str:
    cmpl_proc = subprocess.run(
        ["python", "scripts/eqn2imgurl.py", "README.raw.md", "--print"],
        capture_output=True,
        text=True,
        encoding="utf-8",
    )

    if cmpl_proc.returncode != 0:
        raise RuntimeError("Error transforming README")

    return cmpl_proc.stdout


def main():
    # 1. Format staged C/C++ code
    try:

        for filepath in get_staged_files():
            ext = get_filename_extension(filepath)
            if ext in CLANG_FORMAT_APPLICABLE_FILE_EXTENSIONS:
                subprocess.run(
                    ["clang-format", "-i", "-style=file", filepath]
                ).check_returncode()
                subprocess.run(["git", "add", filepath]).check_returncode()

    except CalledProcessError:
        raise RuntimeError("Format staged C/C++ code failed!")

    try:
        new_readme_text = ""

        # 2. Transform LaTeX math equation in README.raw.md to image url in README.md
        if get_file_status("README.raw.md") in ("M ", "  "):
            new_readme_text = transform_readme()
        else:
            old_readme_text = Path("README.md").read_text(encoding="utf-8")
            stripped = old_readme_text[:old_readme_text.find("## TODO")]
            new_readme_text = stripped

        # 3. Append content of TODO.md and CHANGELOG.md to README.md
        if get_file_status("TODO.md") in ("M ", "  "):
            todo_text = Path("TODO.md").read_text(encoding="utf-8")
            new_readme_text += f"\n## TODO\n\n<details open>\n<summary>TODO</summary>\n\n{todo_text}\n</details>\n"

        if get_file_status("CHANGELOG.md") in ("M ", "  "):
            changelog_text = Path("CHANGELOG.md").read_text(encoding="utf-8")
            new_readme_text += f"\n## CHANGELOG\n\n<details open>\n<summary>CHANGELOG</summary>\n\n{changelog_text}\n</details>\n"

        Path("README.md").write_text(new_readme_text, encoding="utf-8")

        subprocess.run(["git", "add", "README.md"]).check_returncode()

    except CalledProcessError:
        raise RuntimeError("Constructing new README.md failed!")


if __name__ == "__main__":
    main()
