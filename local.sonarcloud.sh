#!/bin/sh

set -x
set -e

echo "Build"
cd converters
cmake -DCOVERAGE=ON .
build-wrapper-linux-x86-64 --out-dir bw-output make clean coverage

echo "Compute gcov reports"
gcovFiles=`find CMakeFiles/tests.dir -name "*.gcda" `
for file in $gcovFiles
do
  gcov $file
done

echo "Scanner"
sonar-scanner \
  -Dsonar.scm.exclusions.disabled=true \
  -Dsonar.cfamily.threads="$(nproc)" \
  -Dsonar.cfamily.cache.enabled=true \
  -Dsonar.cfamily.cache.path=/tmp \
  -Dsonar.organization=airthium \
  -Dsonar.projectKey=Airthium_tanatloc-dockers \
  -Dsonar.sources=src,test \
  -Dsonar.exclusions=src/dxflib/* \
  -Dsonar.host.url=https://sonarcloud.io \
  -Dsonar.cfamily.build-wrapper-output=bw-output \
  -Dsonar.login=efc3fcc7ba5c34a270492718d9aa4d045854e774 \
  -Dsonar.cfamily.gcov.reportsPath=.
