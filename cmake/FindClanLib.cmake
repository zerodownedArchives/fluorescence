# - Find ClanLib
# ClanLib is a cross platform SDK geared toward making games. It is
# available from http://clanlib.org.
#
# The following are defined by this module:
#  ClanLib_FOUND - TRUE if ClanLib was found
#  ClanLib_INCLUDE_DIRS - Directory containing the ClanLib headers
#  ClanLib_LIBRARIES - If invoked via FIND_PACKAGE(ClanLib COMPONENTS ...),
#                      will only contain the libraries matching each component.
#                      otherwise, it will contain all ClanLib libraries found.
# For all components, the following variables are set:
#  ClanLib_${COMPONENT}_LIBRARY - Full path to the component's library.
# 
# Input: 
#   ClanLib_VERSION_LIBNAME - used for finding the libs, 
#           e.g. clanApp22, where "22" is the variable value
#   ClanLib_VERSION_DIRNAME - used to find the include dir, 
#           e.g. /usr/include/ClanLib-2.2, where "2.2" is the variable value
IF(ClanLib_INCLUDE_DIRS)
  SET(ClanLib_FIND_QUIETLY TRUE)
ENDIF(ClanLib_INCLUDE_DIRS)

IF(NOT ClanLib_VERSION_LIBNAME)
  SET(ClanLib_VERSION_LIBNAME "23")
ENDIF(NOT ClanLib_VERSION_LIBNAME)

IF(NOT ClanLib_VERSION_DIRNAME)
  SET(ClanLib_VERSION_DIRNAME "2.3")
ENDIF(NOT ClanLib_VERSION_DIRNAME)

IF(NOT ClanLib_FIND_COMPONENTS)
  SET(ClanLib_FIND_COMPONENTS
    App
    Core
    CSSLayout
    Database
    Display
    GL
    GL1
    GUI
    MikMod
    Network
    RegExp
    Sound
    Sqlite
    SWRender
    Vorbis)
ENDIF(NOT ClanLib_FIND_COMPONENTS)

MACRO(ClanLib_MSG MSG)
  IF(NOT ClanLib_FIND_QUIETLY)
    MESSAGE(STATUS ${MSG})
  ENDIF(NOT ClanLib_FIND_QUIETLY)
ENDMACRO(ClanLib_MSG)

MACRO(ClanLib_ERR MSG)
  IF(ClanLib_FIND_REQUIRED)
    MESSAGE(SEND_ERROR ${MSG})
  ELSE(ClanLib_FIND_REQUIRED)
    ClanLib_MSG(${MSG})
  ENDIF(ClanLib_FIND_REQUIRED)
ENDMACRO(ClanLib_ERR)

MACRO(ClanLib_FIND_COMPONENT COMPONENT)
  ClanLib_MSG("Checking for Clan${COMPONENT}")
  FIND_LIBRARY(ClanLib_${COMPONENT}_LIBRARY clan${ClanLib_VERSION_LIBNAME}${COMPONENT}
    ${CLANLIB_ROOT_DIR}/lib /lib /usr/lib /usr/local/lib
    DOC "Library name for clan${COMPONENT}.")
  IF(ClanLib_${COMPONENT}_LIBRARY)
    SET(ClanLib_${COMPONENT}_FOUND TRUE)
    ClanLib_MSG("Checking for Clan${COMPONENT} -- ${ClanLib_${COMPONENT}_LIBRARY}")
    EXEC_PROGRAM(pkg-config 
        ARGS "--libs" "clan${COMPONENT}-${ClanLib_VERSION_DIRNAME}" 
        OUTPUT_VARIABLE ClanLib_${COMPONENT}_DEPENDENCIES)
  ELSE(ClanLib_${COMPONENT}_LIBRARY)
    SET(ClanLib_${COMPONENT}_FOUND FALSE)
    IF(ClanLib_FIND_REQUIRED_${COMPONENT})
      ClanLib_ERR("Checking for Clan${COMPONENT} -- not found")
    ELSE(ClanLib_FIND_REQUIRED_${COMPONENT})
      ClanLib_MSG("Checking for Clan${COMPONENT} -- not found")
    ENDIF(ClanLib_FIND_REQUIRED_${COMPONENT})
  ENDIF(ClanLib_${COMPONENT}_LIBRARY)
ENDMACRO(ClanLib_FIND_COMPONENT)

ClanLib_MSG("Checking for ClanLib")

FIND_PATH(ClanLib_INCLUDE_DIRS ClanLib/core.h
  ${ClanLib_ROOT_DIR}/include ${ClanLib_ROOT_DIR}/include/ClanLib-${ClanLib_VERSION_DIRNAME}
  /usr/local/include          /usr/local/include/ClanLib-${ClanLib_VERSION_DIRNAME}
  /usr/include                /usr/include/ClanLib-${ClanLib_VERSION_DIRNAME}
  DOC "Where to find the ClanLib includes.")
IF(ClanLib_INCLUDE_DIRS)
  ClanLib_MSG("Checking for ClanLib -- headers")
ELSE(ClanLib_INCLUDE_DIRS)
  ClanLib_ERR("Checking for ClanLib -- headers not found")
ENDIF(ClanLib_INCLUDE_DIRS)

ClanLib_FIND_COMPONENT(Core)
IF(ClanLib_INCLUDE_DIRS AND ClanLib_Core_LIBRARY)
  SET(ClanLib_FOUND TRUE)
  SET(ClanLib_LIBRARIES ${ClanLib_Core_LIBRARY})
ELSE(ClanLib_INCLUDE_DIRS AND ClanLib_Core_LIBRARY)
  SET(ClanLib_FOUND FALSE)
ENDIF(ClanLib_INCLUDE_DIRS AND ClanLib_Core_LIBRARY)

ClanLib_MSG("Checking for other ClanLib components")
FOREACH(COMPONENT ${ClanLib_FIND_COMPONENTS})
  ClanLib_FIND_COMPONENT(${COMPONENT})
  IF(ClanLib_${COMPONENT}_LIBRARY)
#    LIST(APPEND ClanLib_LIBRARIES ${ClanLib_${COMPONENT}_LIBRARY})
    LIST(APPEND ClanLib_LIBRARIES ${ClanLib_${COMPONENT}_DEPENDENCIES})
  ENDIF(ClanLib_${COMPONENT}_LIBRARY)
ENDFOREACH(COMPONENT)

MARK_AS_ADVANCED(
  ClanLib_INCLUDE_DIRS
  ClanLib_App_LIBRARY
  ClanLib_Core_LIBRARY
  ClanLib_Display_LIBRARY
  ClanLib_GL_LIBRARY
  ClanLib_GUI_LIBRARY
  ClanLib_GUIStyleSilver_LIBRARY
  ClanLib_MikMod_LIBRARY
  ClanLib_Network_LIBRARY
  ClanLib_SDL_LIBRARY
  ClanLib_Signals_LIBRARY
  ClanLib_Sound_LIBRARY
  ClanLib_Vorbis_LIBRARY
)
