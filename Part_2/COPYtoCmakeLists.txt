# Created by the script cgal_create_CMakeLists
# This is the CMake script for compiling a set of CGAL applications.

cmake_minimum_required(VERSION 3.1...3.23)

project( optimal_polygon )


# CGAL and its components
find_package( CGAL QUIET COMPONENTS  )

if ( NOT CGAL_FOUND )

  message(STATUS "This project requires the CGAL library, and will not be compiled.")
  return()  

endif()


# Boost and its components
find_package( Boost REQUIRED )

if ( NOT Boost_FOUND )

  message(STATUS "This project requires the Boost library, and will not be compiled.")

  return()  

endif()

# include for optimal_polygon directory
add_subdirectory(Project1-1900082-1800040-1600168)
list(APPEND EXTRA_LIBS utils)
# include for optimal_polygon package


# Creating entries for target: optimal_polygon
# ############################

add_executable( optimal_polygon  main.cpp local.cpp utils_local.cpp utils_simulated_annealing.cpp simulated_annealing.cpp utils_aco.cpp aco.cpp subdivision.cpp sub_incremental.cpp utils_sub.cpp)

add_to_cached_list( CGAL_EXECUTABLE_TARGETS optimal_polygon )

# Link the executable to CGAL and third-party libraries
target_link_libraries(optimal_polygon PRIVATE CGAL::CGAL ${EXTRA_LIBS})