set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/src/cython/)

find_package(PythonInterp 3)
find_package(PythonExtensions REQUIRED)
find_package(Cython REQUIRED)
message(STATUS "PYTHON_EXECUTABLE: ${PYTHON_EXECUTABLE}")
message(STATUS "PYTHON_VERSION_STRING: ${PYTHON_VERSION_STRING}")
message(STATUS "PYTHON_INCLUDE_DIR: ${PYTHON_INCLUDE_DIR}")
message(STATUS "PYTHON_LIBRARY: ${PYTHON_LIBRARY}")
message(STATUS "CYTHON_EXECUTABLE: ${CYTHON_EXECUTABLE}")
message(STATUS "CYTHON_VERSION: ${CYTHON_VERSION}")

set(CYTHON_FLAGS " -I${CMAKE_SOURCE_DIR}/src/cython/ -I${CMAKE_SOURCE_DIR}/include/cython/")
add_cython_target(lgraph_db_python cython/lgraph_db_python.py CXX)
add_library(lgraph_db_python MODULE ${lgraph_db_python})
python_extension_module(lgraph_db_python)
target_link_libraries(lgraph_db_python lgraph libgomp.a crypto dl)