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
import dataclasses
import shutil

import flag


CHALLENGES_BASE_DIR = './challenges'
USERS_HOME_BASE_DIR = '/home/challenges/home/'


@dataclasses.dataclass
class CtfChallenge:
    username: str
    password: str
    home_dir: str
    build_stage_home_dir: str
    challenge_dir: str

    def __post_init__(self):
        self._config = toml.load(os.path.join(self.challenge_dir, 'config.toml'))
        self.flag = flag.generate(self._config["general"]["challenge_name"])

    def create_final_stage_user(self):
        """
        create username that will be used for final stage
        :param build_stage_home_dir: path to user's home directory in build stage
        :param shell: default shell
        :return:
        """
        # salt is None thus will be randomly generated
        encrypted_password = crypt.crypt(self.password)
        shell = self._config["general"]["shell"]
        subprocess.check_call(
            shlex.split(f"useradd -p {encrypted_password} -d {self.home_dir} -s {shell} {self.username}"),
        )

        os.makedirs(self.build_stage_home_dir, exist_ok=True)
        # change home directory's permission
        uid = pwd.getpwnam(self.username).pw_uid
        gid = grp.getgrnam(self.username).gr_gid
        os.chown(self.build_stage_home_dir, uid=uid, gid=gid)
        # folder should not be visible to other users
        os.chmod(self.build_stage_home_dir, 0o760)

    def _generate_flag(self):
        challenge_name = self._config['general']['challenge_name']
        self.flag = flag.generate(challenge_name)

    def _patch_flag(self):
        for file_path in self._config["build"]["files_containing_flag"]:
            flag.patch_file_with_flag(os.path.join(self.challenge_dir, file_path), self.flag)

    def _run_build_scripts(self):
        for script_path in self._config["build"]["build_scripts"]:
            subprocess.check_call(
                shlex.split(f"{os.path.join(self.challenge_dir, script_path)}"),
                shell=True
            )

    def build_challenge(self):
        self._patch_flag()
        self._run_build_scripts()

        # copy description to .description
        shutil.copy2(
            os.path.join(self.challenge_dir, 'description.md'),
            os.path.join(self.build_stage_home_dir, '.description')
        )

        # iterate files to copy
        for file_pair in self._config["build"]["files_to_copy"]:
            # `toml` does not supports unpacking of tables
            srcname, dstname = file_pair.values()
            src = os.path.join(self.challenge_dir, srcname)
            dst = os.path.join(self.build_stage_home_dir, dstname)

            if not os.path.exists(os.path.dirname(dst)):
                os.makedirs(os.path.dirname(dst))

            if (os.path.isdir(src)):
                shutil.copytree(src, dst, dirs_exist_ok=True)
            else:
                shutil.copy2(src, dst)


def create_all_users(challenges_base_dir, users_home_base_dir):
    challenges_directories = \
        [dirname
         for dirname in os.listdir(challenges_base_dir)
         if os.path.isdir(os.path.join(challenges_base_dir, dirname))]

    # make home base directory
    os.makedirs(users_home_base_dir, exist_ok=True)

    # sort challenges just in case
    challenges_directories.sort()

    # list of users. list ends each user maps to a dict containing the d
    users = []

    # collect flags, usernames and home directories
    for challenge_index, challenge_directory in enumerate(challenges_directories):
        # password should be the flag of the last stage
        if challenge_index == 0:
            password = 'none'
        else:
            password = users[challenge_index - 1].flag

        # generate flag for each challenge
        username = f'challenge{challenge_directory}'
        user = CtfChallenge(
            username=username,
            password=password,
            home_dir=os.path.join('/home', username),
            build_stage_home_dir=os.path.join(users_home_base_dir, username),
            challenge_dir=os.path.join(challenges_base_dir, challenge_directory)
        )

        user.create_final_stage_user()
        user.build_challenge()
        users.append(user)


if __name__ == "__main__":
    create_all_users(CHALLENGES_BASE_DIR, USERS_HOME_BASE_DIR)

