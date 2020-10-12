# OpenCascade 7.4.0
FROM ubuntu:18.10

WORKDIR /usr/bin

COPY ./converters .

RUN mkdir build

RUN cmake ..

RUN make
