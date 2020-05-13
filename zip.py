#!/usr/bin/env python3

import os
import shutil
from tempfile import TemporaryDirectory

import click

config = {
    "README.assignment.pdf": "README.pdf",
    "test_integrate.py": "test.py",
    "concated.cpp": "lzw.cpp",
    "Makefile.assignment": "Makefile",
}


@click.command()
@click.argument("zipfilename")
def main(zipfilename):
    with TemporaryDirectory() as d:
        for k, v in config.items():
            shutil.copy(k, os.path.join(d, v))
        shutil.make_archive(zipfilename, "zip", d)


if __name__ == "__main__":
    main()
