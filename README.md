# tanatloc-dockers

Dockers for the Tanatloc project.

## Requirements

You need a valid `GITHUB_TOKEN` defined in your environment
variables to retrieve the archives OpenCascade and Gmsh from
this repository.

```shell
export GITHUB_TOKEN=xxx
```

> https://github.com/settings/tokens

## Usage

```shell
./build.sh opencascade
./build.sh gmsh
./build.sh converters
```

## Test

```shell
./build.sh gmsh:test
```
