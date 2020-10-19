#!/usr/bin/python3

import getpass
import pwd
import grp
import sys


def get_all_ctf_users() -> list:
    users = [p.pw_name for p in pwd.getpwall() if p.pw_name.startswith('challenge')]
    # sort users by name
    users.sort()
    return users



def get_next_challenge_user():
    current_user = getpass.getuser()
    assert(current_user.startswith('challenge'))
    all_users = get_all_ctf_users()
    user_index = all_users.index(current_user)
    if user_index + 1 >= len(all_users):
        raise ValueError("Last challenge")
    return all_users[user_index + 1]
    

if __name__ == "__main__":
    try:
        print(get_next_challenge_user())
    except (ValueError, AssertionError):
        sys.exit(1)
