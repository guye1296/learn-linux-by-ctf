FROM ubuntu:latest AS base

# install required packages
RUN apt update
RUN apt install -y python3 python3-pip

COPY requirements.txt /tmp/
RUN python3 -m pip install -r /tmp/requirements.txt && rm /tmp/requirements.txt

# TODO: figure out on how to install challenge dependencies that are needed for runtime



# -- build challeges --

FROM  base AS challenges
COPY challenges/ challenges/

# create home directories and internal directories
RUN mkdir -p /home/challenges/home
RUN mkdir -p /home/challenges/internal

COPY scripts/ scripts/
# install script requirements
RUN python3 -m pip install -r scripts/requirements.txt
RUN apt install make gcc
RUN cd scripts && make

# create users and store flags
RUN chmod +x scripts/create_users.py
RUN ./scripts/create_users.py


# -- final image --

# copy build output from images
FROM base AS output

# copy internal challenge files (description, hints) to internal folder
COPY --from=challenges /home/challenges/internal /internal/

# copy home directorie
COPY --from=challenges /home/challenges/home/ /home/

# copy created users
COPY --from=challenges /etc/passwd /etc/passwd
COPY --from=challenges /etc/shadow /etc/shadow
COPY --from=challenges /etc/group /etc/group
COPY --from=challenges /etc/gshadow /etc/gshadow

RUN chmod -R 400 /internal

# copy essential scripts to path
COPY --chown=root:root  scripts/display-current-challenge.sh /usr/bin/display-challenge
COPY --chown=root:root scripts/get_next_challenge.py /usr/bin/_get-next-challenge
COPY --from=challenges /scripts/guess_flag /usr/bin/_guess-flag
COPY --from=challenges /scripts/guess_flag.sh /usr/bin/_guess-flag.sh

# allow execute permission to everyone
RUN chmod a+x /usr/bin/display-challenge
# set /etc/profile to display challenge upon login
RUN sed -i '1s;^;alias flag="exec /usr/bin/_guess-flag.sh"\ndisplay-challenge\n\n;' /etc/bash.bashrc

# set user and workdir to first challenge

ENV PROMPT_CHALLENGE=1

USER challenge00
WORKDIR /home/challenge00
ENTRYPOINT /bin/bash
