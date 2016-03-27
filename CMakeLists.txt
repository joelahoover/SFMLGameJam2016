# This version might be wrong
cmake_minimum_required(VERSION 2.6)
project(OrangeGuy)

# Enable debug symbols by default
if(CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Add the sources and includes
add_subdirectory(Source)
include_directories(Source)
if(APPLE)
    add_subdirectory(osx)
    include_directories(osx)
else()
    add_subdirectory(other)
    include_directories(other)
endif()

# Add the includes

# Setup the executable
set(EXECUTABLE_NAME "OrangeGuy")
if(APPLE)
    add_executable(${EXECUTABLE_NAME} MACOSX_BUNDLE ${PROJECT_SOURCES})
else()
    add_executable(${EXECUTABLE_NAME} ${PROJECT_SOURCES})
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

# Add libraries
# SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED system window graphics audio)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})
endif()

# Create a stand-alone bundle on OS X
if(APPLE)
    set(MAKE_INSTALL_NAME_DIR @rpath)

    # Add Apple libraries
    FIND_LIBRARY(FOUNDATION_LIBRARY Foundation)
    target_link_libraries(${EXECUTABLE_NAME} ${FOUNDATION_LIBRARY})

    # Add Info.plist to bundle
    set_target_properties(${EXECUTABLE_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/osx/Info.plist.in)

    # Copy assets into the bundle
    set(BUNDLE_PATH ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_NAME}.app)
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/Source/data DESTINATION ${BUNDLE_PATH}/Contents/Resources)
    install(FILES ${PROJECT_SOURCE_DIR}/osx/Icon.icns DESTINATION ${BUNDLE_PATH}/Contents/Resources)

    # Make the bundle drag-n-drop
    get_filename_component(SFML_LIBRARY_DIR ${SFML_SYSTEM_LIBRARY} DIRECTORY)
    set(LIBRARIES_DIR ${THOR_LIBRARY_DIR};${SFML_LIBRARY_DIR})
    # Copy freetype manually, since it otherwise just causes problems (I think because it uses @executable_path instead of @rpath)
    install(DIRECTORY ${SFML_LIBRARY_DIR}/freetype.framework DESTINATION ${BUNDLE_PATH}/Contents/Frameworks)
    # Let BundleUtilities do everything else automaitcally
    install(CODE "
        include(BundleUtilities)
        fixup_bundle(\"${BUNDLE_PATH}\" \"\" \"${LIBRARIES_DIR}\")
        " COMPONENT Runtime)
endif()
