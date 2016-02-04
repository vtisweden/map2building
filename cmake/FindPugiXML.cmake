# - Find PugiXML
# Find the PugiXML source directory.
#
#  PUGIXML_INCLUDE_DIR - where to find OVR.h, etc.
#  PUGIXML_FOUND        - True if PugiXML found.
#  PUGIXML_VERSION      - Version of the PugiXML if found

# Look for the header file.
FIND_PATH(PUGIXML_INCLUDE_DIR NAMES pugiconfig.hpp HINTS 
	${CMAKE_SOURCE_DIR}/externals/pugixml/src )


# Try to ascertain the version of the SDK
IF(PUGIXML_INCLUDE_DIR)
	SET(_PUGIXML_VERSION_FILE "${PUGIXML_INCLUDE_DIR}/pugixml.hpp")
	IF(EXISTS "${_PUGIXML_VERSION_FILE}") 
		FILE(STRINGS "${_PUGIXML_VERSION_FILE}" _PUGIXML_VERSION_FILE_CONTENTS REGEX "#[ \t]?define PUGIXML_VERSION[ \t]+[0-9]+")

		STRING(REGEX REPLACE ".*#[ \t]?define PUGIXML_VERSION[ \t]+([0-9]+).*" "\\1" PUGIXML_VERSION_PRODUCT ${_PUGIXML_VERSION_FILE_CONTENTS})
		
		SET(PUGIXML_VERSION "${PUGIXML_VERSION_PRODUCT}" CACHE INTERNAL "The version of PugiXML which was detected")
		
	ENDIF()
ENDIF()

# handle the QUIETLY and REQUIRED arguments and set PUGIXML_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PugiXML DEFAULT_MSG PUGIXML_INCLUDE_DIR)

MARK_AS_ADVANCED(PUGIXML_INCLUDE_DIR)
