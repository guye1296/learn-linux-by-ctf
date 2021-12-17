#!/bin/bash

if [ $# -lt 2 ]; then
    >&2 echo USAGE: $0 [IMAGE_NAME] [DEPENDENCIES]...
    exit 1
fi


DOCKER_IMAGE_NAME=$1
declare -a DEPENDENCIES_PATHS=${@:2}

image_creation_time=$(docker inspect -f '{{ .Created }}' $DOCKER_IMAGE_NAME)
image_creation_time_epoch=$(date -d "$image_creation_time" +%s)

for dependency in $DEPENDENCIES_PATHS
do
    dependency_modification_epoch=$(date -r "$dependency" +%s)
    if [ "$image_creation_time_epoch" -lt "$dependency_modification_epoch" ];
    then
        docker build -t buildroot .
        exit 0
    fi
done
