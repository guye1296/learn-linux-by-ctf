#!/bin/sh

ln -s /build $PWD/output
make BR2_EXTERNAL=/buildroot-v86 v86_defconfig && make -j8
