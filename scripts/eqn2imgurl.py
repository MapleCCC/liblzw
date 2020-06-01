#!/usr/bin/env python3

import re
import urllib
from pathlib import Path
from pprint import PrettyPrinter
from typing import AnyStr, Match

import click

pprint = PrettyPrinter().pprint

# TODO: handle case when inline equation spans multiple lines.
# TODO: improve regular expression so that inline equation pattern doesn't match block equations

# Use + instead of * because we don't want to match $$
INLINE_EQUATION_PATTERN = re.compile(r"\$(.+?)\$")
BLOCK_EQUATION_PATTERN = re.compile(r"\$\$(.+?)\$\$", re.DOTALL)

RENDER_SERVER_HOST_URL = "https://latex.codecogs.com/"

# TODO: what kind of img format is suitable?
RENDER_IMG_FORMAT = "svg"

GLOBAL_RENDER_CONFIG = r"\fn_cm&space;\small&space;"


class ConflictOptionsError(Exception):
    pass


def preprocess(math_expr: str) -> str:
    math_expr = math_expr.strip()
    math_expr = math_expr.replace("\n", " ")
    escaped = urllib.parse.quote(math_expr, safe=r"/{}()=*")  # type: ignore
    return escaped


def math_repl(matchobj: Match, mode="inline") -> AnyStr:
    if mode == "inline":
        render_config = r"\inline&space;" + GLOBAL_RENDER_CONFIG
    elif mode == "block":
        render_config = GLOBAL_RENDER_CONFIG
    else:
        raise ValueError("Invalid mode")

    math_expr = matchobj.group(1)
    preprocessed = preprocess(math_expr)
    link = f"{RENDER_SERVER_HOST_URL}{RENDER_IMG_FORMAT}.latex?{render_config}{preprocessed}"
    img_struct = f"![{math_expr}]({link})"
    return f"[{img_struct}]({link})"


def inline_math_repl(matchobj: Match) -> AnyStr:
    return math_repl(matchobj, mode="inline")


def block_math_repl(matchobj: Match) -> AnyStr:
    return math_repl(matchobj, mode="block")


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
