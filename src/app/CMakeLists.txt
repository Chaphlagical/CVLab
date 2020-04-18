Ubpa_GlobGroupSrcs(RST sources PATHS
	${CMAKE_CURRENT_SOURCE_DIR}
)
Ubpa_AddTarget(MODE "EXE" SOURCES ${src} LIBS "glfw")
