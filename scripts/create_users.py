#!/usr/bin/python3

import os
import sys
import toml
import subprocess
import shlex
import crypt
import random
import pwd
import grp

import generate_flag


CHALLENGES_BASE_DIR = './challenges'
USERS_HOME_BASE_DIR = '/home/challenges/home/'
FLAGS_FILE_PATH = '/flags'
DEFAULT_SHELL = '/bin/bash'


def linux_create_user(username, password, home_dir, shell):
    # salt is None thus will be randomly generated
    encrypted_password = crypt.crypt(password)
    os.makedirs(home_dir)
    subprocess.check_call(
        shlex.split(f"useradd -p {encrypted_password} -d {home_dir} -s {shell} {username}"),
    )
    # change home directory's permission
    uid = pwd.getpwnam(username).pw_uid
    gid = grp.getgrnam(username).gr_gid
    os.chown(home_dir, uid=uid, gid=gid)
    # folder should not be visible to other users
    os.chmod(home_dir, 0o760)


def get_challenge_name(challenge_dir: str) -> tuple:
    config = toml.load(os.path.join(challenge_dir, 'config.toml'))
    return config['general']['challenge_name']


def create_all_users(challenges_base_dir, flags_path, users_home_base_dir):
    challenges_directories = \
        [dirname
         for dirname in os.listdir(challenges_base_dir)
         if os.path.isdir(os.path.join(challenges_base_dir, dirname))]

    # make home base directory
    os.makedirs(challenges_base_dir, exist_ok=True)

    with open(flags_path, 'w') as flags:
        for challenge_directory in challenges_directories:
            # generate flag for each challenge
            challenge_name = get_challenge_name(os.path.join(challenges_base_dir, challenge_directory))
            username = f'challenge{challenge_directory}'
            flag = generate_flag.generate_flag(challenge_name)
            flags.write(f'{username}: {flag}\n')

            home_dir = os.path.join('/home', username)
            linux_create_user(username, flag, home_dir, DEFAULT_SHELL)


if __name__ == "__main__":
    create_all_users(CHALLENGES_BASE_DIR, FLAGS_FILE_PATH, USERS_HOME_BASE_DIR)

