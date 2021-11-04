## PRE ##
FROM ubuntu:22.04 as builder

ENV DEBIAN_FRONTEND noninteractive

# Install packages
RUN apt update \
  && apt upgrade -yq

RUN apt install -yq \
  automake \
  bison \
  catch2 cmake \
  file flex \
  g++ gfortran git \
  libfftw3-dev libgl1-mesa-dev libgsl-dev \
  libhdf5-dev liblapack-dev libnlopt-dev \
  libopenblas-dev libxi-dev libxmu-dev \
  make mesa-common-dev \
  patch pkg-config python3-minimal python3-distutils \
  tcl-dev tk-dev \
  unzip \
  wget

RUN apt autoremove \
  && apt clean \
  && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*
