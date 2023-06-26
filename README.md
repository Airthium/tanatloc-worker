# tanatloc-worker

Docker for the Tanatloc project.

Converters are defined in the [tanatloc/converters](https://github.com/Airthium/tanatloc-converters) repository.

## Usage

If you want to build the entire worker:

```shell
./build.sh worker
```

You can also build only some parts of the project:

```shell
./build.sh opencascade
./build.sh gmsh
./build.sh freefem
./build.sh vtk
./build.sh converters
```
