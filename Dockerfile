FROM ubuntu:latest AS base

# install required packages
RUN apt update
RUN apt install -y python3 python3-pip

COPY requirements.txt /tmp/
RUN python3 -m pip install -r /tmp/requirements.txt && rm /tmp/requirements.txt

# TODO: figure out on how to install challenge dependencies that are needed for runtime

# copy essential scripts to path
COPY --chown=root:root  scripts/display-current-challenge.sh /usr/bin/display-challenge
# allow execute permission to everyone
RUN chmod a+x /usr/bin/display-challenge


# -- build challeges --

FROM  base AS challenges
COPY challenges/ challenges/

# create home directories and internal directories
RUN mkdir -p /home/challenges/home
RUN mkdir -p /home/challenges/internal

# copy script requirements
COPY scripts/requirements.txt .
RUN python3 -m pip install -r requirements.txt

# create users and store flags
COPY scripts/create_users.py .
COPY scripts/flag.py .
RUN chmod +x create_users.py
RUN ./create_users.py


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

# copy system-wide binaries to host
COPY scripts/display-current-challenge.sh /usr/bin/display-challenge

# set /etc/profile to display challenge upon login
RUN sed -i '1s;^;display-challenge\n\n;' /etc/bash.bashrc

# set user and workdir to first challenge
USER challenge00
WORKDIR /home/challenge00
ENTRYPOINT /bin/bash
