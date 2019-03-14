# vim: noexpandtab:ts=4:sts=4:sw=4

cmake_minimum_required (VERSION 3.0.2)
project (climbell)

set (SRC
	src/main.c
	src/curse.c
	src/course_creation.c
	)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug build")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -Wextra")
endif()

find_package(Git)
if(Git_FOUND)
	if(EXISTS "${CMAKE_SOURCE_DIR}/.git")

		execute_process(
			COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
			WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
			OUTPUT_VARIABLE _git_rev
			ERROR_QUIET
			OUTPUT_STRIP_TRAILING_WHITESPACE
			)

		message("Adding git rev ${_git_rev} to version number")

		add_definitions(-DGIT_REV="${_git_rev}")
	else()
		message("Not adding git rev to version number")
	endif()
endif()

FIND_PACKAGE(PkgConfig)
PKG_CHECK_MODULES(GLIB glib-2.0)

include_directories(${GLIB_INCLUDE_DIRS})
link_directories(${GLIB_LIBRARY_DIRS})
add_definitions(${GLIB_CFLAGS_OTHER})

add_executable (climbell ${SRC} ${Glib_LIBRARY})

target_link_libraries (climbell glib-2.0 ncurses)

install(TARGETS
	climbell
	DESTINATION
	bin
)
