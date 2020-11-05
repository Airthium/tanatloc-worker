FROM ubuntu:20.10 as build

LABEL maintainer="https://github.com/orgs/tanatloc/people"

ENV DEBIAN_FRONTEND noninteractive

# Install packages
RUN apt-get update \
  && apt-get install -yq \
  catch2 cmake g++ gfortran git \
  libopenblas-dev liblapack-dev \
  libgl1-mesa-dev libxi-dev libxmu-dev \
  mesa-common-dev tcl-dev tk-dev wget \
  && apt-get autoremove \
  && apt-get clean \
  && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*
