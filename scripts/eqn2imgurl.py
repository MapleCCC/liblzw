#!/usr/bin/env python3

import re
import urllib
from pathlib import Path
from pprint import PrettyPrinter
from typing import AnyStr, Match

import click

pprint = PrettyPrinter().pprint

# TODO: handle case when inline equation spans multiple lines.

# Use + instead of * because we don't want to match $$
INLINE_EQUATION_PATTERN = re.compile(r"\$(.+?)\$")
BLOCK_EQUATION_PATTERN = re.compile(r"\$\$(.+?)\$\$", re.DOTALL)

RENDER_SERVER_HOST_URL = "https://latex.codecogs.com/"

RENDER_IMG_FORMAT = "svg"

GLOBAL_RENDER_CONFIG = r"\fn_cm&space;\small&space;"


class ConflictOptionsError(Exception):
    pass


def cleanup(string: str) -> str:
    string = string.strip()
    string = string.replace("\n", " ")
    escaped = urllib.parse.quote(string, safe=r"/\{}()=*")  # type: ignore
    return escaped


def inline_math_repl(matchobj: Match) -> AnyStr:
    math_exp = matchobj.group(1)
    cleaned = cleanup(math_exp)
    render_config = r"\inline&space;" + GLOBAL_RENDER_CONFIG
    return f"![]({RENDER_SERVER_HOST_URL}{RENDER_IMG_FORMAT}.latex?{render_config}{cleaned})"


def block_math_repl(matchobj: Match) -> AnyStr:
    math_exp = matchobj.group(1)
    cleaned = cleanup(math_exp)
    render_config = GLOBAL_RENDER_CONFIG
    return f"![]({RENDER_SERVER_HOST_URL}{RENDER_IMG_FORMAT}.latex?{render_config}{cleaned})"


@click.command()
@click.argument("file")
@click.argument("encoding", default="utf-8")
@click.option("-o", "--output")
@click.option("-i", "--in-place", is_flag=True, default=False)
@click.option("-p", "--print", "print_to_stdout", is_flag=True, default=False)
def main(
    file: str, encoding: str, output: str, in_place: bool, print_to_stdout: bool
) -> None:
    if len(list(filter(None, (output, in_place, print_to_stdout)))) > 1:
        raise ConflictOptionsError(
            "--output, --in-place, and --print-to-stdout are mutually exclusive options"
        )

    in_file = Path(file)

    content = in_file.read_text(encoding)

    # WARNING: replace block equations before replacing inline equation.
    # Otherwise inline equation pattern also recognizes block equation pattern.
    intermediate = re.sub(BLOCK_EQUATION_PATTERN, block_math_repl, content)
    new_content = re.sub(INLINE_EQUATION_PATTERN, inline_math_repl, intermediate)

    if in_place:
        in_file.write_text(new_content, encoding)
    elif output is not None:
        Path(output).write_text(new_content, encoding)
    elif print_to_stdout:
        print(new_content)
    else:
        raise RuntimeError(
            "You need to specify one of --output, --in-place, or --print"
        )


if __name__ == "__main__":
    main()
