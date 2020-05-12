# OpenCascade 7.4.0
FROM ubuntu:18.04

WORKDIR /usr/bin

COPY ./converters .

RUN cmake .

RUN make
