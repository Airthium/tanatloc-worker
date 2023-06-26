#!/usr/bin/env bash

declare -A ARCHIVES

# Get the list of releases and associated assets:
#> curl -u "https://api.github.com/repos/Airthium/tanatloc-dockers/releases"
ARCHIVES["opencascade-7.6.0.tgz"]="https://api.github.com/repos/Airthium/tanatloc-dockers/releases/assets/65069748"
ARCHIVES["gmsh-4.10.1-source.tgz"]="https://api.github.com/repos/Airthium/tanatloc-dockers/releases/assets/65069747"
ARCHIVES["FreeFem-sources-4.11.tar.gz"]="https://api.github.com/repos/Airthium/tanatloc-dockers/releases/assets/65069742"
ARCHIVES["VTK-9.1.0.tar.gz"]="https://api.github.com/repos/Airthium/tanatloc-dockers/releases/assets/65069731"

declare -A TARGETS

TARGETS["opencascade"]="pre opencascade.build"
TARGETS["gmsh"]="pre opencascade.build gmsh.build"
TARGETS["gmsh:test"]="pre opencascade.build gmsh.build gmsh.test"
TARGETS["freefem"]="pre freefem.build"
TARGETS["vtk"]="pre vtk.build"
TARGETS["converters"]="pre freefem.build vtk.build opencascade.build gmsh.build converters.build"
TARGETS["worker"]="pre freefem.build vtk.build opencascade.build gmsh.build converters.build release"

DOCKERFILE_PATH="./Dockerfile"

usage() {
  echo "Usage: ./build.sh [target]"
  echo "target=[opencascade, gmsh, freefem, vtk, converters or worker]"
}

checkArchives() {
  echo -e "Checking archives..."
  for archive in "${!ARCHIVES[@]}"; do
    echo -e "> $archive"
    downloadArchive "$archive"
  done
}

downloadArchive() {
  archive="$1"

  # Download target if not already present
  if [ ! -f "$archive" ]; then
    echo "File $archive not found. Downloading..."
    curl -LJO -H 'Accept: application/octet-stream' ${ARCHIVES[$archive]}
  fi
}

buildDockerfile() {
  target="$1"
  echo -e "Building Dockerfile..."
  echo -e "> ${DOCKERFILE_PATH}"

  fragments="${TARGETS[$target]}"

  rm -f $DOCKERFILE_PATH
  for fragment in $fragments; do
    if [ ! -f $DOCKERFILE_PATH ]; then
      cat "./${fragment}.Dockerfile" >$DOCKERFILE_PATH
    else
      cat "./${fragment}.Dockerfile" >>$DOCKERFILE_PATH
    fi
  done
}

build() {
  target="$1"
  echo -e "Building Docker image..."
  echo -e "> docker build $ARGS -f $DOCKERFILE_PATH . -t tanatloc/$target:latest"
  docker build "$ARGS" -f $DOCKERFILE_PATH . -t "tanatloc/$target:latest"
}

target=$1
shift

if [ "${TARGETS[$target]}" == "" ]; then
  echo "Invalid target"
  usage
  exit 1
fi

ARGS="$*"
echo "Building started..."
echo "TARGET=\"$target\""
echo "ARGS=\"$ARGS\""

checkArchives
buildDockerfile "$target"
