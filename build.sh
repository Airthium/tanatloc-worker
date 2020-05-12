#!/usr/bin/env bash

declare -A ARCHIVES

ARCHIVES["opencascade-7.4.0.tar.gz"]="https://github.com/Airthium/tanatloc-dockers/releases/download/opencascade-7.4.0/opencascade-7.4.0.tar.gz"
ARCHIVES["gmsh.tar.gz"]="https://github.com/Airthium/tanatloc-dockers/releases/download/gmsh-4.5.6/gmsh.tar.gz"

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
    # TODO wget is not working for private repository releases
    # Use? curl -vLJO -H 'Authorization: token my_access_token' 'https://api.github.com/repos/:owner/:repo/releases/assets/:id'
    wget ${ARCHIVES[$archive]};
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
  docker build "$@" -f $DOCKERFILE_PATH .
}

target=$1
shift

if [ "${TARGETS[$target]}" == "" ]; then
  echo "Invalid target";
  usage
  exit 1;
fi

echo "TARGET=\"$target\""
echo "ARGS=\"$@\""

checkArchives
buildDockerfile "$target"
build

# echo "Building docker image..."
# cat ./pre.Dockerfile > /tmp/Dockerfile
# cat ./opencascade.build.Dockerfile | sed s/FROM.*// >> /tmp/Dockerfile
# cat ./gmsh.build.Dockerfile | sed s/FROM.*// >> /tmp/Dockerfile

# docker build "$@" -f /tmp/Dockerfile .
