FROM ubuntu:20.10 as builder

ENV DEBIAN_FRONTEND noninteractive

# Install packages
RUN apt-get update \
  && apt-get install -yq \
  automake \
  bison \
  catch2 cmake \
  flex \
  g++ gfortran git \
  libfftw3-dev libgl1-mesa-dev libgsl-dev \
  libhdf5-dev liblapack-dev libnlopt-dev \
  libopenblas-dev libxi-dev libxmu-dev \
  make mesa-common-dev \
  patch pkg-config python3-minimal python3-distutils \
  tcl-dev tk-dev \
  unzip \
  wget \
  && apt-get autoremove \
  && apt-get clean \
  && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*
