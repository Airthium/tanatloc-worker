## PRE ##
FROM ubuntu:23.04 as builder

ENV DEBIAN_FRONTEND noninteractive

# Install packages
RUN apt clean \
  && apt update \
  && apt upgrade -yq \
  && apt install -yq \
  automake \
  bison \
  catch cmake \
  file flex \
  g++ gfortran git \
  libfftw3-dev libgl1-mesa-dev libgsl-dev \
  libhdf5-dev liblapack-dev libnlopt-dev \
  libopenblas-dev libxi-dev libxmu-dev \
  make mesa-common-dev mpich \
  patch pkg-config python3-minimal python3-distutils \
  rapidjson-dev \
  tcl-dev tk-dev \
  unzip \
  wget \
  && apt autoremove \
  && apt clean \
  && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*
