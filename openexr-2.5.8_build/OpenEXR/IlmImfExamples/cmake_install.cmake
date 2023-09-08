# Install script for directory: /home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImfExamples

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/aahartl/6040/build")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/OpenEXR/examples" TYPE FILE FILES
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImfExamples/main.cpp"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImfExamples/drawImage.cpp"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImfExamples/rgbaInterfaceExamples.cpp"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImfExamples/rgbaInterfaceTiledExamples.cpp"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImfExamples/generalInterfaceExamples.cpp"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImfExamples/lowLevelIoExamples.cpp"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImfExamples/previewImageExamples.cpp"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImfExamples/generalInterfaceTiledExamples.cpp"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImfExamples/generalInterfaceTiledExamples.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImfExamples/drawImage.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImfExamples/rgbaInterfaceExamples.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImfExamples/generalInterfaceExamples.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImfExamples/rgbaInterfaceTiledExamples.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImfExamples/lowLevelIoExamples.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImfExamples/previewImageExamples.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImfExamples/namespaceAlias.h"
    )
endif()

