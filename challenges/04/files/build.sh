#!/bin/bash

pushd .
cd "$(dirname "$0")"
make
popd
