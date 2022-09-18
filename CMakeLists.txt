cmake_minimum_required(VERSION 3.18.0)

project(archimedes VERSION 1.0.0)

set(CMAKE_CONFIGURATION_TYPES "Debug;RelWithDebInfo" CACHE STRING "" FORCE)

include_directories(
	src
)

set(util_SRCS
)
source_group(util FILES ${util_SRCS})
set_source_files_properties(${util_SRCS} PROPERTIES UNITY_GROUP "util")

set(archimedes_SRCS
	${util_SRCS}
)

set(util_HDRS
	src/util/log_util.h
)
source_group(util FILES ${util_HDRS})

set(archimedes_HDRS
	${util_HDRS}
	src/pch.h
)

set(game_test_SRCS
#	test/game/game_test.cpp
)
source_group(game FILES ${game_test_SRCS})

set(archimedes_test_SRCS
	${game_test_SRCS}
	test/main.cpp
)

find_package(Boost 1.69.0 REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
add_definitions(-DBOOST_DATE_TIME_NO_LIB)

#Qt5 modules
#different modules have different licenses, make sure all modules used here are compatible with the GPL 2.0
set(CMAKE_AUTOMOC ON)
find_package(Qt5 5.14 COMPONENTS Core REQUIRED) #licensed under the GPL 2.0 (as well as the LGPL 3.0)
find_package(Qt5 5.14 COMPONENTS Gui REQUIRED) #licensed under the GPL 2.0 (as well as the LGPL 3.0)
find_package(Qt5 5.14 COMPONENTS Widgets REQUIRED) #licensed under the GPL 2.0 (as well as the LGPL 3.0)
find_package(Qt5 5.14 COMPONENTS Multimedia REQUIRED) #licensed under the GPL 2.0 (as well as the LGPL 3.0)
find_package(Qt5 5.14 COMPONENTS Location REQUIRED) #licensed under the GPL 2.0 (as well as the LGPL 3.0)
find_package(Qt5 5.14 COMPONENTS Qml REQUIRED) #licensed under the GPL 2.0 (as well as the LGPL 3.0)
find_package(Qt5 5.14 COMPONENTS Quick REQUIRED) #licensed under the GPL 2.0 (as well as the LGPL 3.0)

set(QT_LIBRARIES
	Qt5::Core
	Qt5::Gui
	Qt5::Widgets
	Qt5::Multimedia
	Qt5::Location
	Qt5::Qml
	Qt5::Quick
)

set(QT_LIBRARIES ${QT_LIBRARIES} Qt5::LocationPrivate)

set(archimedes_LIBS
	${QT_LIBRARIES}
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
	add_compile_options(/W4 /w44800 /wd4244 /wd4458)
else()
	add_compile_options(-Wall -Wno-unknown-pragmas)
endif()

add_library(archimedes STATIC ${archimedes_SRCS} ${archimedes_HDRS})

add_executable(archimedes_test ${archimedes_test_SRCS})
add_test(archimedes_test archimedes_test)
enable_testing()

target_precompile_headers(archimedes PRIVATE src/pch.h)

set_target_properties(archimedes PROPERTIES UNITY_BUILD_MODE GROUP)

set_target_properties(archimedes_test PROPERTIES UNITY_BUILD_MODE GROUP)
set_source_files_properties(${game_test_SRCS} PROPERTIES UNITY_GROUP "game_test")

target_link_libraries(archimedes ${archimedes_LIBS})

target_precompile_headers(archimedes_test REUSE_FROM archimedes)

target_link_libraries(archimedes_test LINK_PUBLIC archimedes)
