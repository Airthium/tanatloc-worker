#!/bin/sh

cd converters
cmake .
build-wrapper-linux-x86-64 --out-dir bw-output make clean all
sonar-scanner \
  -Dsonar.organization=airthium \
  -Dsonar.projectKey=Airthium_tanatloc-dockers \
  -Dsonar.sources=. \
  -Dsonar.exclusions=dxflib/* \
  -Dsonar.host.url=https://sonarcloud.io \
  -Dsonar.cfamily.build-wrapper-output=bw-output \
  -Dsonar.login=efc3fcc7ba5c34a270492718d9aa4d045854e774