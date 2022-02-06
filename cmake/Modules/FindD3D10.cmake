# Attempt to find the D3D10 libraries
# Defines:
#
#  D3D10_FOUND		  - system has D3D10
#  D3D10_INCLUDE_PATH - path to the D3D10 headers
#  D3D10_LIBRARIES	  - path to the D3D10 libraries
#  D3D10_LIB		  - d3d10.lib
#  D3DX10_LIB		  - d3dx10.lib
#  DXERR10_LIB		  - dxerr10.lib

set(D3D10_FOUND "NO")

if (WIN32)
  set(WIN8_SDK_DIR "C:/Program Files (x86)/Windows Kits/8.1")
  set(LEGACY_SDK_DIR "$ENV{DXSDK_DIR}")

  if (CMAKE_CL_64)
    set(ARCH x64)
  else (CMAKE_CL_64)
    set(ARCH x86)
  endif (CMAKE_CL_64)

  # Look for the windows 8 sdk
  find_path(D3D10_INCLUDE_PATH
    NAMES d3d10.h
    PATHS "${WIN8_SDK_DIR}/Include/um"
    NO_DEFAULT_PATH
    DOC "Path to the windows 8 d3d10.h file"
    )

  if (D3D10_INCLUDE_PATH)
    find_library(D3D10_LIB
      NAMES d3d10
      #PATHS "${WIN8_SDK_DIR}/Lib/win8/um/${ARCH}"
      PATHS "${WIN8_SDK_DIR}/Lib/winv6.3/um/${ARCH}"
      NO_DEFAULT_PATH
      DOC "Path to the windows 8 d3d10.lib file"
      )

    if (D3D10_LIB)
      set(D3D10_FOUND "YES")
      set(D3D10_LIBRARIES ${D3D10_LIB})
      mark_as_advanced(D3D10_INCLUDE_PATH D3D10_LIB)
    endif (D3D10_LIB)
  endif (D3D10_INCLUDE_PATH)

  # Otherwise look for legacy installs
  if (NOT D3D10_FOUND)
    set(D3D10_INCLUDE_PATH NOTFOUND)

    find_path(D3D10_INCLUDE_PATH
      NAMES d3d10.h
      PATHS "${LEGACY_SDK_DIR}/Include"
      NO_DEFAULT_PATH
      DOC "Path to the legacy d3d10.h file"
      )

    if (D3D10_INCLUDE_PATH)
      find_library(D3D10_LIB
        NAMES d3d10
        PATHS "${LEGACY_SDK_DIR}/Lib/${ARCH}"
        NO_DEFAULT_PATH
        DOC "Path to the legacy d3d10.lib file"
        )

      find_library(D3DX10_LIB
        NAMES d3dx10
        PATHS "${LEGACY_SDK_DIR}/Lib/${ARCH}"
        NO_DEFAULT_PATH
        DOC "Path to the legacy d3dx10.lib file"
        )

      find_library(DXERR10_LIB
        NAMES dxerr dxerr10
        PATHS "${LEGACY_SDK_DIR}/Lib/${ARCH}"
        NO_DEFAULT_PATH
        DOC "Path to the legacy dxerr10x.lib file"
        )

      if (D3D10_LIB AND D3DX10_LIB AND DXERR10_LIB)
        set(D3D10_FOUND "YES")
        set(D3D10_LIBRARIES ${D3D10_LIB} ${D3DX10_LIB} ${DXERR10_LIB})
        mark_as_advanced(D3D10_INCLUDE_PATH D3D10_LIB D3DX10_LIB DXERR10_LIB)
      endif (D3D10_LIB AND D3DX10_LIB AND DXERR10_LIB)
    endif (D3D10_INCLUDE_PATH)
  endif (NOT D3D10_FOUND)
endif (WIN32)

if (D3D10_FOUND)
  if (NOT D3D10_FIND_QUIETLY)
    message(STATUS "D3D10 headers found at ${D3D10_INCLUDE_PATH}")
  endif (NOT D3D10_FIND_QUIETLY)
else (D3D10_FOUND)
  if (D3D10_FIND_REQUIRED)
    message(FATAL_ERROR "Could NOT find Direct3D11")
  endif (D3D10_FIND_REQUIRED)
  if (NOT D3D10_FIND_QUIETLY)
    message(STATUS "Could NOT find Direct3D11")
  endif (NOT D3D10_FIND_QUIETLY)
endif (D3D10_FOUND)

