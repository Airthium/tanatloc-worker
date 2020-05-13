#!/usr/bin/env bash

declare -A ARCHIVES

# Get the list of releases and associated assets:
#> curl -u $GITHUB_TOKEN:x-oauth-basic "https://api.github.com/repos/Airthium/tanatloc-dockers/releases"
ARCHIVES["opencascade-7.4.0.tar.gz"]="https://api.github.com/repos/Airthium/tanatloc-dockers/releases/assets/20660980"
ARCHIVES["gmsh.tar.gz"]="https://api.github.com/repos/Airthium/tanatloc-dockers/releases/assets/20664165"

declare -A TARGETS

TARGETS["opencascade"]="pre opencascade.build"
TARGETS["gmsh"]="pre opencascade.build gmsh.build"
TARGETS["gmsh:test"]="pre opencascade.build gmsh.build gmsh.test"
TARGETS["converters"]="pre opencascade.build gmsh.build converters.build"

DOCKERFILE_PATH="/tmp/Dockerfile";

usage() {
  echo "Usage: ./build.sh [target]";
  echo "target=[opencascade, gmsh or converters]"
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
      cat ./${fragment}.Dockerfile | sed s/FROM.*// >> $DOCKERFILE_PATH
    fi
  done
}

build() {
  echo -e "Building Docker image..."
  echo -e "> docker build $ARGS -f $DOCKERFILE_PATH ."
  docker build $ARGS -f $DOCKERFILE_PATH .
}

target=$1
shift

if [ "${TARGETS[$target]}" == "" ]; then
  echo "Invalid target";
  usage
  exit 1;
fi

ARGS="$@";
echo "TARGET=\"$target\""
echo "ARGS=\"$ARGS\""

checkArchives
buildDockerfile "$target"
build

# echo "Building docker image..."
# cat ./pre.Dockerfile > /tmp/Dockerfile
# cat ./opencascade.build.Dockerfile | sed s/FROM.*// >> /tmp/Dockerfile
# cat ./gmsh.build.Dockerfile | sed s/FROM.*// >> /tmp/Dockerfile

# docker build "$@" -f /tmp/Dockerfile .
