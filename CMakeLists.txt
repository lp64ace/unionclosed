cmake_minimum_required(VERSION 3.5)
project(UnionClosedConjecture)

# -----------------------------------------------------------------------------
# Redirect Outputs

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# -----------------------------------------------------------------------------
# Add Sub-Directories

add_subdirectory(src)
