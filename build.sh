#!/usr/bin/env bash

# Check GITHUB_TOKEN
if [ -z "$GITHUB_TOKEN" ]
then
  echo "GITHUB_TOKEN variable is empty"
  exit 1
fi

declare -A ARCHIVES

# Get the list of releases and associated assets:
#> curl -u $GITHUB_TOKEN:x-oauth-basic "https://api.github.com/repos/Airthium/tanatloc-dockers/releases"
ARCHIVES["opencascade-7.4.0.tar.gz"]="https://api.github.com/repos/Airthium/tanatloc-dockers/releases/assets/20660980"
ARCHIVES["gmsh.tar.gz"]="https://api.github.com/repos/Airthium/tanatloc-dockers/releases/assets/27930881"
ARCHIVES["FreeFem-sources-4.8.tar.gz"]="https://api.github.com/repos/Airthium/tanatloc-dockers/releases/assets/31462514"
ARCHIVES["VTK-9.0.1.tar.gz"]="https://api.github.com/repos/Airthium/tanatloc-dockers/releases/assets/29680338"

declare -A TARGETS

TARGETS["opencascade"]="pre opencascade.build"
TARGETS["gmsh"]="pre opencascade.build gmsh.build"
TARGETS["gmsh:test"]="pre opencascade.build gmsh.build gmsh.test"
TARGETS["freefem"]="pre freefem.build"
TARGETS["vtk"]="pre vtk.build"
TARGETS["converters"]="pre freefem.build vtk.build opencascade.build gmsh.build converters.build"
TARGETS["worker"]="pre freefem.build vtk.build opencascade.build gmsh.build converters.build release"

DOCKERFILE_PATH="/tmp/Dockerfile";

# Read Github Token from 2 different env variables for CI:
GITHUB_TOKEN=${BUILD_TOKEN:-${GITHUB_TOKEN}}

usage() {
  echo "Usage: ./build.sh [target]";
  echo "target=[opencascade, gmsh, freefem, vtk, converters or worker]"
}

checkArchives() {
  echo -e "Checking archives..."
  for archive in "${!ARCHIVES[@]}"; do
    echo -e "> $archive";
    downloadArchive "$archive";
  done
}

downloadArchive() {
  archive="$1"

  # Download target if not already present
  if [ ! -f $archive ]; then
    echo "File $archive not found. Downloading..."
    curl -LJO -u $GITHUB_TOKEN:x-oauth-basic -H 'Accept: application/octet-stream' ${ARCHIVES[$archive]}
  fi
}

buildDockerfile() {
  target="$1"
  echo -e "Building Dockerfile..."
  echo -e "> ${DOCKERFILE_PATH}"

  declare -A fragments
  fragments="${TARGETS[$target]}"

  rm $DOCKERFILE_PATH;
  for fragment in $fragments; do
    if [ ! -f $DOCKERFILE_PATH ]; then
      cat ./${fragment}.Dockerfile > $DOCKERFILE_PATH
    else
      cat ./${fragment}.Dockerfile >> $DOCKERFILE_PATH
    fi
  done
}

build() {
  target="$1"
  echo -e "Building Docker image..."
  echo -e "> docker build $ARGS -f $DOCKERFILE_PATH . -t tanatloc/$target"
  docker build $ARGS -f $DOCKERFILE_PATH . -t tanatloc/$target
}

target=$1
shift

if [ "${TARGETS[$target]}" == "" ]; then
  echo "Invalid target";
  usage
  exit 1;
fi

ARGS="$@";
echo "Building started..."
echo "TARGET=\"$target\""
echo "ARGS=\"$ARGS\""

checkArchives
buildDockerfile "$target"
build "$target"
