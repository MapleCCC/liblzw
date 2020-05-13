#!/usr/bin/env python3

import click


@click.command()
@click.argument("filename")
def main(filename: str) -> None:
    with open(filename, "rb") as f:
        print(f.read())


if __name__ == "__main__":
    main()
