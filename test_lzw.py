import os
import random
import string
import subprocess
from tempfile import TemporaryDirectory

import pytest
from hypothesis import given
from hypothesis.strategies import integers

VALID_CHARSET = (
    string.ascii_letters + string.digits + string.punctuation + string.whitespace
)

MAX_FILE_LEN = 7000
MAX_NUM_TEST_FILES = 10

BASELINE_EXE = os.path.join(os.getcwd(), "lzw_example_win.exe")
EXPERIMENT_EXE = os.path.join(os.getcwd(), "lzw.exe")


def read_file_content(filename: str) -> str:
    with open(filename, "r", encoding="utf-8") as f:
        return f.read()


def diff_file(filename1: str, filename2: str) -> int:
    content1 = read_file_content(filename1)
    content2 = read_file_content(filename2)
    return content1 == content2


def generate_gibberish_file(filename: str) -> None:
    content = "".join(random.choices(VALID_CHARSET, k=random.randrange(MAX_FILE_LEN)))
    with open(filename, "w", encoding="utf-8") as f:
        f.write(content)


# There are different testing strategies. Our easy way is to set the random seed.
@given(integers())
def test_compression(seed: int) -> None:
    random.seed(seed)

    with TemporaryDirectory() as directory:
        os.chdir(directory)

        test_files = [f"file{i}" for i in range(random.randrange(MAX_NUM_TEST_FILES))]
        for test_file in test_files:
            generate_gibberish_file(test_file)

        subprocess.run([EXPERIMENT_EXE, "-c", "output.lzw"] + test_files)
        subprocess.run([BASELINE_EXE, "-c", "output_example.lzw"] + test_files)

        assert diff_file("output.lzw", "output_example.lzw")


@pytest.mark.xfail(reason="Not Yet Implemented")
def test_decompress():
    raise NotImplementedError
