cmake_minimum_required(VERSION 3.0)

    project(Tanatloc)

        ##LIBRARIES
#OpenCASCADE
    find_package(OpenCASCADE) if (OpenCASCADE_FOUND)
        include_directories(${OpenCASCADE_INCLUDE_DIR})
            set(LIBS ${LIBS} ${OpenCASCADE_LIBRARY_DIR})
                set(OCC_LIBS ${OpenCASCADE_LIBRARIES}) endif(OpenCASCADE_FOUND)

#DXF library
                    AUX_SOURCE_DIRECTORY(dxflib DXF_SOURCE)

#Gmsh library
                        AUX_SOURCE_DIRECTORY(gmsh GMSH_SOURCE)

#OCC library
                            AUX_SOURCE_DIRECTORY(occ OCC_SOURCE)

#ThreeJS library
                                AUX_SOURCE_DIRECTORY(threeJS THREEJS_SOURCE)

                                    ##SOURCES
#DXFToBRep
    set(DXFToBRepSOURCE DXFToBRep.cpp DXFConverter.cpp ${DXF_SOURCE})

#BRepToThreeJS
        set(BRepToThreeJSSOURCE BRepToThreeJS.cpp ${OCC_SOURCE} ${
            THREEJS_SOURCE})

#GmshToThreeJS
            set(GmshToThreeJSSOURCE GmshToThreeJS.cpp ${GMSH_SOURCE} ${
                THREEJS_SOURCE})

#StepToThreeJS(TODO to remove)
                set(StepToThreeJSSOURCE StepToThreeJS.cpp ${OCC_SOURCE} ${
                    THREEJS_SOURCE})

#StepUnion(TODO to replace by BRepUnion)
                    set(StepUnionSOURCE StepUnion.cpp ${OCC_SOURCE})

                        ##Executables
#DXFToBRep
    add_executable(DXFToBRep ${DXFToBRepSOURCE})
        target_link_libraries(DXFToBRep ${OCC_LIBS})

#BRepToThreeJS
            add_executable(BRepToThreeJS ${BRepToThreeJSSOURCE})
                target_link_libraries(BRepToThreeJS ${OCC_LIBS})

#GmshToThreeJS
                    add_executable(GmshToThreeJS ${GmshToThreeJSSOURCE})

#StepToThreeJS
                        add_executable(StepToThreeJS ${StepToThreeJSSOURCE})
                            target_link_libraries(StepToThreeJS ${OCC_LIBS})

#StepUnion
                                add_executable(StepUnion ${StepUnionSOURCE})
                                    target_link_libraries(StepUnion ${OCC_LIBS})
