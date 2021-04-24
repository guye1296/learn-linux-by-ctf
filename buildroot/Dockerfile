FROM ubuntu:latest

# buildroot version to use
ARG BUILDROOT_VERSION=2021.02-rc2

RUN apt-get update

# install dependencies
RUN apt-get install -y \
    sed \
    make \
    binutils \
    build-essential\
    gcc \
    g++ \
    bash \
    patch \
    gzip \
    bzip2 \
    perl \
    tar \
    cpio \
    unzip \
    rsync \
    file \
    bc \
    python \
    wget \
    libssl-dev

# fetch buildroot
WORKDIR /home
RUN wget -c http://buildroot.org/downloads/buildroot-${BUILDROOT_VERSION}.tar.gz; \
    tar -xvf buildroot-${BUILDROOT_VERSION}.tar.gz;

VOLUME /out

WORKDIR /home/buildroot-${BUILDROOT_VERSION}

# copy local files
COPY ./buildroot-v86 /buildroot-v86

ENTRYPOINT ["/buildroot-v86/build.sh"]
