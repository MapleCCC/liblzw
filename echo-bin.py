#!/usr/bin/env python3

import string
from typing import Iterable, List

import click
from more_itertools import grouper


def even_cut_str(x: str, n: int, fillvalue: str = "\0") -> Iterable[str]:
    return map("".join, grouper(x, n, fillvalue))


def hex2byte(x: str) -> bytes:
    def numfy(d: str):
        if d in string.digits:
            return int(d)
        else:
            return ord(d) - 65 + 10

    assert len(x) == 2 and x[0] in string.hexdigits and x[1] in string.hexdigits
    integer = 16 * numfy(x[0]) + numfy(x[1])
    return integer.to_bytes(1, "big")


@click.command()
@click.argument("filename")
@click.argument("byte_sequence", nargs=-1)
def main(filename: str, byte_sequence: List[str]) -> None:
    with open(filename, "wb") as f:
        for hex_ in even_cut_str("".join(byte_sequence), 2):
            f.write(hex2byte(hex_))


if __name__ == "__main__":
    main()
