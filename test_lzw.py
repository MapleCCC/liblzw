import os
import shutil
import subprocess
import uuid
from pathlib import Path
from random import sample
from typing import List

from hypothesis import example, given, settings
from hypothesis.strategies import binary, builds, lists

# TODO: compare speed of Python implem and C++ implem of LZW algorithm
# TODO: deploy pytest-subtesthack
# TODO: randomize test file names
# TODO: randomize lzw file names
# TODO: draw histogram to visualize speed vs text size. (time complexity)
# TODO: update Pytest and plugins
# TODO: use pathlib to simplify code

EXAMPLE_EXE = os.path.join(os.getcwd(), "lzw_example_win.exe")
EXPERIMENT_EXE = os.path.join(os.getcwd(), "build", "lzw")

MAX_TEST_FILE_LEN = 10000
MAX_NUM_TEST_FILES = 3  # TODO: increase number of test files


TEST_FILES_BUILD_STRATEGY = lists(
    binary(max_size=MAX_TEST_FILE_LEN), min_size=1, max_size=MAX_NUM_TEST_FILES,
)

# All possible one-length bytes
VALID_CHARSET = [i.to_bytes(1, "big") for i in range(256)]
EXAMPLE_TEXT_TEST_CODE_DICT_OVERFLOW = b"".join(
    b"".join(sample(VALID_CHARSET, k=256)) for _ in range(20)
)


@given(l=TEST_FILES_BUILD_STRATEGY)
@example(l=[EXAMPLE_TEXT_TEST_CODE_DICT_OVERFLOW] * 3)
@settings(deadline=None)
def test_integrate(l: List[bytes], tmp_path: Path) -> None:
    # We need to intentionally create a unique subpath for each function invocation
    # Because every hypothesis' example of the test function share the same
    # tmp_path fixture instance, which is undesirable for some test cases.
    subpath = tmp_path / str(uuid.uuid4())
    subpath.mkdir()
    os.chdir(subpath)

    test_files = [f"file{i}" for i in range(len(l))]
    for test_file, s in zip(test_files, l):
        with open(test_file, "wb") as f:
            f.write(s)

    subprocess.run([EXPERIMENT_EXE, "-c", "a.lzw"] + test_files).check_returncode()

    for test_file in test_files:
        shutil.move(test_file, test_file + "old")

    subprocess.run([EXPERIMENT_EXE, "-d", "a.lzw"]).check_returncode()

    for test_file in test_files:
        assert open(test_file, "rb").read() == open(test_file + "old", "rb").read()
