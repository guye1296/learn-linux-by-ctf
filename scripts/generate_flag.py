#!/usr/bin/python3

import sys
import random
import string


RANDOM_SUFFIX_LENGTH = 8


def generate_flag(challenge_name: str) -> str:
    # flag structure will be appending `RANDOM_SUFFIX_LENGTH` random letter
    # to the challenge name
    random_suffix = ''.join([random.choice(string.ascii_letters) for _ in range(RANDOM_SUFFIX_LENGTH)])
    flag = f"{challenge_name}_{random_suffix}"
    return flag


# generate_flag.py [CHALLENGE_NAME]
if __name__ == "__main__":
    if len(sys.argv) != 2:
        sys.stderr.write(f"usage: {sys.argv[0]} [CHALLENGE_NAME]\n")
        sys.exit(1)

    print(generate_flag(sys.argv[1]))
    sys.exit(0)
