#!/usr/bin/env bash

docker run \
    --privileged \
    --rm \
    --name build-v86 \
    -v $PWD/out:/out \
    -v $PWD/build:/build \
    -v $PWD/buildroot-v86/:/buildroot-v86 \
    buildroot

echo "See ./out/ for built ISO"
