# OpenCascade 7.4.0
FROM ubuntu:20.10

WORKDIR /root/converters

COPY ./converters .

# Build converters
RUN cmake . \
  && make -j "$(nproc)"
