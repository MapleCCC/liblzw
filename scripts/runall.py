#!/usr/bin/env python3

import click
import subprocess
from typing import List


@click.command()
@click.argument("executables", nargs=-1)
def main(executables: List[str]):
    for executable in executables:
        subprocess.run([executable]).check_returncode()


if __name__ == "__main__":
    main()
