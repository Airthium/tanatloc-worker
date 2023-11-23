#!/usr/bin/env bash

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

buildDockerfile() {
	target="$1"
	echo -e "Building Dockerfile..."
	echo -e "> ${DOCKERFILE_PATH}"

	fragments="${TARGETS[$target]}"

	rm -f $DOCKERFILE_PATH
	for fragment in $fragments; do
		if [ ! -f $DOCKERFILE_PATH ]; then
			cat "./${fragment}.Dockerfile" >$DOCKERFILE_PATH
			echo "" >>$DOCKERFILE_PATH
		else
			cat "./${fragment}.Dockerfile" >>$DOCKERFILE_PATH
			echo "" >>$DOCKERFILE_PATH
		fi
	done
}

build() {
	target="$1"
	echo -e "Building Docker image..."
	echo -e "> docker build . -t tanatloc/$target:latest $ARGS"
	docker build . -t "tanatloc/$target:latest" "$ARGS"
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

buildDockerfile "$target"
