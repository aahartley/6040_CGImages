# Install script for directory: /home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf

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
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libIlmImf-2_5.so.26.0.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libIlmImf-2_5.so.26"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "\$ORIGIN/../lib:/home/aahartl/6040/build/lib")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/home/aahartl/6040/openexr-2.5.8_build/OpenEXR/IlmImf/libIlmImf-2_5.so.26.0.0"
    "/home/aahartl/6040/openexr-2.5.8_build/OpenEXR/IlmImf/libIlmImf-2_5.so.26"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libIlmImf-2_5.so.26.0.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libIlmImf-2_5.so.26"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHANGE
           FILE "${file}"
           OLD_RPATH "/home/aahartl/6040/openexr-2.5.8_build/IlmBase/Imath:/home/aahartl/6040/openexr-2.5.8_build/IlmBase/IlmThread:/home/aahartl/6040/openexr-2.5.8_build/IlmBase/Half:/home/aahartl/6040/openexr-2.5.8_build/IlmBase/IexMath:/home/aahartl/6040/openexr-2.5.8_build/IlmBase/Iex:"
           NEW_RPATH "\$ORIGIN/../lib:/home/aahartl/6040/build/lib")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libIlmImf-2_5.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libIlmImf-2_5.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libIlmImf-2_5.so"
         RPATH "\$ORIGIN/../lib:/home/aahartl/6040/build/lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/aahartl/6040/openexr-2.5.8_build/OpenEXR/IlmImf/libIlmImf-2_5.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libIlmImf-2_5.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libIlmImf-2_5.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libIlmImf-2_5.so"
         OLD_RPATH "/home/aahartl/6040/openexr-2.5.8_build/IlmBase/Imath:/home/aahartl/6040/openexr-2.5.8_build/IlmBase/IlmThread:/home/aahartl/6040/openexr-2.5.8_build/IlmBase/Half:/home/aahartl/6040/openexr-2.5.8_build/IlmBase/IexMath:/home/aahartl/6040/openexr-2.5.8_build/IlmBase/Iex:"
         NEW_RPATH "\$ORIGIN/../lib:/home/aahartl/6040/build/lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libIlmImf-2_5.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OpenEXR" TYPE FILE FILES
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfForward.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfExport.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfAttribute.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfBoxAttribute.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfCRgbaFile.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfChannelList.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfChannelListAttribute.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfCompressionAttribute.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfDoubleAttribute.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfFloatAttribute.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfFrameBuffer.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfHeader.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfIO.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfInputFile.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfIntAttribute.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfLineOrderAttribute.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfMatrixAttribute.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfOpaqueAttribute.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfOutputFile.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfRgbaFile.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfStringAttribute.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfVecAttribute.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfHuf.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfWav.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfLut.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfArray.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfCompression.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfLineOrder.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfName.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfPixelType.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfVersion.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfXdr.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfConvert.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfPreviewImage.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfPreviewImageAttribute.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfChromaticities.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfChromaticitiesAttribute.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfKeyCode.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfKeyCodeAttribute.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfTimeCode.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfTimeCodeAttribute.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfRational.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfRationalAttribute.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfFramesPerSecond.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfStandardAttributes.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfStdIO.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfEnvmap.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfEnvmapAttribute.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfInt64.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfRgba.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfTileDescription.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfTileDescriptionAttribute.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfTiledInputFile.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfTiledOutputFile.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfTiledRgbaFile.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfRgbaYca.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfTestFile.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfThreading.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfB44Compressor.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfStringVectorAttribute.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfMultiView.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfAcesFile.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfMultiPartOutputFile.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfGenericOutputFile.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfMultiPartInputFile.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfGenericInputFile.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfPartType.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfPartHelper.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfOutputPart.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfTiledOutputPart.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfInputPart.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfTiledInputPart.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfDeepScanLineOutputFile.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfDeepScanLineOutputPart.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfDeepScanLineInputFile.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfDeepScanLineInputPart.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfDeepTiledInputFile.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfDeepTiledInputPart.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfDeepTiledOutputFile.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfDeepTiledOutputPart.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfDeepFrameBuffer.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfDeepCompositing.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfCompositeDeepScanLine.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfNamespace.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfDeepImageState.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfDeepImageStateAttribute.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImf/ImfFloatVectorAttribute.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  execute_process(COMMAND /usr/bin/cmake -E chdir "$ENV{DESTDIR}/home/aahartl/6040/build/lib" /usr/bin/cmake -E create_symlink libIlmImf-2_5.so libIlmImf.so)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  message("-- Creating symlink in  libIlmImf.so -> libIlmImf-2_5.so")
endif()

