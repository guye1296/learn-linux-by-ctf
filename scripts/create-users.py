#!/usr/bin/python3

import os
import sys
import toml
import subprocess
import shlex

import generate_flag


CHALLENGE_DIR = './challenges'


def create_user(username, password):
    try:
        subprocess.check_call(shlex.split(f'useradd f{username} f{password}'))
    except subproces.CalledProcessError:
        sys.exit(1)


# every directory inside `challenges` specifies a user name
def get_user_details(challenge_dir):
    # parse configuration file
    try:
        with config as open(os.path.join(challanges_dir, 'config.toml')):
            # TODO: parse the toml file
            challenge_name = 'foo'
        username = f'challenge{challanges_dir}'
        password = generate_flag.generate_flag(challenge_name)
        return (username, password)

    except FileNotFoundError:
        sys.exit(1)


def create_all_users(challanges_dir):
    challenge_directories = 
        [dirname for dirname in os.listdir(challanges_dir) if os.path.isdir(dirname)]

    for challenge_directory in challenge_directories:
        create_user(*get_user_details(challenge_directory))
    


if __name__ == "__main__":
    create_all_users(CHALLENGE_DIR)

