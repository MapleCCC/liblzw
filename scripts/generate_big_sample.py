from random import sample

from LZW.utils import ascii2byte

# All possible one-length bytes
VALID_CHARSET = [ascii2byte(i) for i in range(256)]
EXAMPLE_TEXT_TEST_CODE_DICT_OVERFLOW = b"".join(
    b"".join(sample(VALID_CHARSET, k=256)) for _ in range(10000)
)

with open("big-sample.txt", "wb") as f:
    f.write(EXAMPLE_TEXT_TEST_CODE_DICT_OVERFLOW)
