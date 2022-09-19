cmake_minimum_required(VERSION 3.18.0)

set(CMAKE_CONFIGURATION_TYPES "Debug;RelWithDebInfo" CACHE STRING "" FORCE)

include_directories(
	src
)

set(map_SRCS
	src/map/map_projection.cpp
	src/map/mercator_map_projection.cpp
)
source_group(map FILES ${map_SRCS})
set_source_files_properties(${map_SRCS} PROPERTIES UNITY_GROUP "map")

set(util_SRCS
	src/util/angle_util.cpp
	src/util/assert_util.cpp
	src/util/astronomy_util.cpp
	src/util/color_container.cpp
	src/util/date_util.cpp
	src/util/event_loop.cpp
	src/util/exception_util.cpp
	src/util/geocircle_util.cpp
	src/util/geocoordinate.cpp
	src/util/geojson_util.cpp
	src/util/geopath_util.cpp
	src/util/geoshape_util.cpp
	src/util/number_util.cpp
	src/util/point_container.cpp
	src/util/point_util.cpp
	src/util/random.cpp
	src/util/rect_util.cpp
	src/util/string_conversion_util.cpp
	src/util/string_util.cpp
	src/util/thread_pool.cpp
	src/util/time_util.cpp
)
source_group(util FILES ${util_SRCS})
set_source_files_properties(${util_SRCS} PROPERTIES UNITY_GROUP "util")

set(archimedes_SRCS
	${map_SRCS}
	${util_SRCS}
)

set(map_HDRS
	src/map/equirectangular_map_projection.h
	src/map/map_projection.h
	src/map/mercator_map_projection.h
)
source_group(map FILES ${map_HDRS})

set(util_HDRS
	src/util/angle_util.h
	src/util/assert_util.h
	src/util/astronomy_util.h
	src/util/char_util.h
	src/util/color_container.h
	src/util/container_random_util.h
	src/util/container_util.h
	src/util/date_util.h
	src/util/enum_util.h
	src/util/event_loop.h
	src/util/exception_util.h
	src/util/fractional_int.h
	src/util/geocircle_util.h
	src/util/geocoordinate.h
	src/util/geojson_util.h
	src/util/geopath_util.h
	src/util/georectangle.h
	src/util/geoshape_util.h
	src/util/list_util.h
	src/util/locale_util.h
	src/util/log_util.h
	src/util/map_util.h
	src/util/number_util.h
	src/util/path_util.h
	src/util/point_container.h
	src/util/point_operators.h
	src/util/point_util.h
	src/util/queue_util.h
	src/util/qunique_ptr.h
	src/util/qvariant_util.h
	src/util/random.h
	src/util/rect_util.h
	src/util/set_util.h
	src/util/singleton.h
	src/util/size_operators.h
	src/util/size_util.h
	src/util/string_conversion_util.h
	src/util/string_util.h
	src/util/string_view_util.h
	src/util/thread_pool.h
	src/util/time_util.h
	src/util/type_traits.h
	src/util/vector_random_util.h
	src/util/vector_util.h
)
source_group(util FILES ${util_HDRS})

set(archimedes_HDRS
	${map_HDRS}
	${util_HDRS}
	src/archimedes.h
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

option(WITH_GEOJSON "Compile with support for generating map data from GeoJSON files" OFF)

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

if(WITH_GEOJSON)
	add_definitions(-DUSE_GEOJSON)
	set(QT_LIBRARIES ${QT_LIBRARIES} Qt5::LocationPrivate)
endif()

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
