#!/usr/bin/python3

import sys
import random
import string
import jinja2


RANDOM_SUFFIX_LENGTH = 4


def generate(challenge_name: str) -> str:
    # flag structure will be appending `RANDOM_SUFFIX_LENGTH` random letter
    # to the challenge name
    random_suffix = ''.join([random.choice(string.ascii_letters) for _ in range(RANDOM_SUFFIX_LENGTH)])
    flag = f"{challenge_name}_{random_suffix}"
    return flag


# patches file in place
def patch_file_with_flag(path: str, flag: str):
    with open(path, 'r') as template_file:
        template_content = template_file.read()

    template = jinja2.Template(template_content)
    patched_content = template.render(flag=flag)

    # patch file in-place
    with open(path, 'w') as patched:
        patched.write(patched_content)
