FROM ubuntu:latest AS base

# install required packages
RUN apt-update
RUN apt install -y python3


COPY requirements.txt /tmp/
RUN python3 -m pip install /tmp/requirements.txt && rm /tmp/requirements.txt

# TODO: figure out on how to install challenge dependencies that are needed for runtime

# copy essentials scripts to path
COPY --chown=root:root  scripts/display-current-challenge.sh /usr/bin/display-challenge
# allow execute permission to everyone
RUN chmod a+x /usr/bin/display-challenge


# -- build challeges --

FROM  base AS challenges
COPY challenges/ challenges/

# copy script requirements
COPY scripts/requirements.txt .
RUN python -m pip install -r requirements.txt

# create users
COPY scripts/create-users.py .
RUN chmod +x create-users.py
RUN ./create-users.py


# -- final image --

# copy build output from images
FROM base AS output

# set user and workdir to first challenge
USER challenge00
WORKDIR /home/challenge00
ENTRYPOINT /bin/bash
