SET(FREETYPE_FOUND "NO")
IF (WIN32)
	find_path(FREETYPE_INCLUDE_DIR 
		NAMES ft2build.h
		PATHS 
        "D:/3rd-party/freetype-2.13.0/include"
		)
	message(STATUS "FREETYPE_INCLUDE_DIR => ${FREETYPE_INCLUDE_DIR}")
	find_path(FREETYPE_LIB_DIR 
		NAMES freetype.lib
		PATHS 
        "D:/3rd-party/freetype-2.13.0/lib"
		)
	message(STATUS "FREETYPE_LIB_DIR => ${FREETYPE_LIB_DIR}")
	find_library(FREETYPE_LIBRARY 
			NAMES freetype
			PATHS
            "D:/3rd-party/freetype-2.13.0/lib"
			)
	message(STATUS "FREETYPE_LIBRARY => ${FREETYPE_LIBRARY}")
	# debug release same library
	find_library(FREETYPE_LIBRARY_DEBUG
			NAMES freetyped
			PATHS
            "D:/3rd-party/freetype-2.13.0/lib"
			)
	message(STATUS "FREETYPE_LIBRARY_DEBUG => ${FREETYPE_LIBRARY_DEBUG}")
ELSE (WIN32)
# I have not touched any Linux or Mac OS projects, however a explicit framework or architecture is prevailing

#	find_path(GLUT_INCLUDE_DIR 
#		
#	...	
ENDIF (WIN32)


IF(FREETYPE_INCLUDE_DIR AND FREETYPE_LIB_DIR AND FREETYPE_LIBRARY AND FREETYPE_LIBRARY_DEBUG)
  SET(FREETYPE_FOUND "YES")
ENDIF(FREETYPE_INCLUDE_DIR AND FREETYPE_LIB_DIR AND FREETYPE_LIBRARY AND FREETYPE_LIBRARY_DEBUG)