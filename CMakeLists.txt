cmake_minimum_required(VERSION 3.18.0)

set(CMAKE_CONFIGURATION_TYPES "Debug;RelWithDebInfo" CACHE STRING "" FORCE)

include_directories(
	src
)

set(database_SRCS
	src/database/basic_data_entry.cpp
	src/database/data_entry.cpp
	src/database/data_entry_history.cpp
	src/database/data_module.cpp
	src/database/data_module_container.cpp
	src/database/database.cpp
	src/database/defines_base.cpp
	src/database/gsml_data.cpp
	src/database/gsml_parser.cpp
	src/database/gsml_property.cpp
	src/database/named_data_entry.cpp
	src/database/predefines.cpp
)
source_group(database FILES ${database_SRCS})
set_source_files_properties(${database_SRCS} PROPERTIES UNITY_GROUP "database")

set(language_SRCS
	src/language/grammatical_gender.cpp
	src/language/language.cpp
	src/language/name_variant.cpp
	src/language/word.cpp
	src/language/word_type.cpp
)
source_group(language FILES ${language_SRCS})
set_source_files_properties(${language_SRCS} PROPERTIES UNITY_GROUP "language")

set(map_SRCS
	src/map/map_projection.cpp
	src/map/mercator_map_projection.cpp
)
source_group(map FILES ${map_SRCS})
set_source_files_properties(${map_SRCS} PROPERTIES UNITY_GROUP "map")

set(time_SRCS
	src/time/calendar.cpp
	src/time/timeline.cpp
)
source_group(time FILES ${time_SRCS})
set_source_files_properties(${time_SRCS} PROPERTIES UNITY_GROUP "time")

set(util_SRCS
	src/util/angle_util.cpp
	src/util/assert_util.cpp
	src/util/astronomy_util.cpp
	src/util/color_container.cpp
	src/util/colorization_type.cpp
	src/util/date_util.cpp
	src/util/enum_converter.cpp
	src/util/event_loop.cpp
	src/util/exception_util.cpp
	src/util/geocircle_util.cpp
	src/util/geocoordinate.cpp
	src/util/geojson_util.cpp
	src/util/geopath_util.cpp
	src/util/geoshape_util.cpp
	src/util/image_provider_base.cpp
	src/util/image_util.cpp
	src/util/number_util.cpp
	src/util/point_container.cpp
	src/util/point_util.cpp
	src/util/random.cpp
	src/util/rect_util.cpp
	src/util/string_conversion_util.cpp
	src/util/string_util.cpp
	src/util/text_processor_base.cpp
	src/util/thread_pool.cpp
	src/util/time_util.cpp
)
source_group(util FILES ${util_SRCS})
set_source_files_properties(${util_SRCS} PROPERTIES UNITY_GROUP "util")

set(archimedes_SRCS
	${database_SRCS}
	${language_SRCS}
	${map_SRCS}
	${time_SRCS}
	${util_SRCS}
)

set(database_HDRS
	src/database/basic_data_entry.h
	src/database/data_entry.h
	src/database/data_entry_history.h
	src/database/data_module.h
	src/database/data_module_container.h
	src/database/data_type.h
	src/database/data_type_metadata.h
	src/database/database.h
	src/database/defines_base.h
	src/database/gsml_data.h
	src/database/gsml_data_visitor.h
	src/database/gsml_element_visitor.h
	src/database/gsml_operator.h
	src/database/gsml_parser.h
	src/database/gsml_property.h
	src/database/gsml_property_visitor.h
	src/database/named_data_entry.h
	src/database/predefines.h
)
source_group(database FILES ${database_HDRS})

set(language_HDRS
	src/language/grammatical_gender.h
	src/language/language.h
	src/language/language_family.h
	src/language/name_variant.h
	src/language/word.h
	src/language/word_type.h
)
source_group(language FILES ${language_HDRS})

set(map_HDRS
	src/map/equirectangular_map_projection.h
	src/map/map_projection.h
	src/map/mercator_map_projection.h
)
source_group(map FILES ${map_HDRS})

set(time_HDRS
	src/time/calendar.h
	src/time/day_of_the_week.h
	src/time/month.h
	src/time/timeline.h
)
source_group(time FILES ${time_HDRS})

set(util_HDRS
	src/util/angle_util.h
	src/util/assert_util.h
	src/util/astronomy_util.h
	src/util/char_util.h
	src/util/color_container.h
	src/util/colorization_type.h
	src/util/container_random_util.h
	src/util/container_util.h
	src/util/date_util.h
	src/util/enum_converter.h
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
	src/util/image_provider_base.h
	src/util/image_util.h
	src/util/list_util.h
	src/util/locale_util.h
	src/util/log_output_handler.h
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
	src/util/text_processor_base.h
	src/util/thread_pool.h
	src/util/time_util.h
	src/util/type_traits.h
	src/util/vector_random_util.h
	src/util/vector_util.h
)
source_group(util FILES ${util_HDRS})

set(archimedes_HDRS
	${database_HDRS}
	${language_HDRS}
	${map_HDRS}
	${time_HDRS}
	${util_HDRS}
	src/archimedes.h
	src/pch.h
)

set(util_test_SRCS
	test/util/angle_test.cpp
	test/util/astronomy_test.cpp
	test/util/geocoordinate_test.cpp
	test/util/number_test.cpp
	test/util/random_test.cpp
	test/util/string_conversion_test.cpp
	test/util/time_test.cpp
)
source_group(util FILES ${util_test_SRCS})

set(archimedes_test_SRCS
	${util_test_SRCS}
	test/main.cpp
)

option(WITH_GEOJSON "Compile with support for generating map data from GeoJSON files" OFF)

if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.18.0")
	#unity builds using defined groups require CMake 3.18
	option(ENABLE_UNITY_BUILD "Use unity builds for compilation" ON)
endif()

find_package(Boost 1.69.0 REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
add_definitions(-DBOOST_DATE_TIME_NO_LIB)

#Qt5 modules
#different modules have different licenses, make sure all modules used here are compatible with the MIT license
set(CMAKE_AUTOMOC ON)
find_package(Qt5 5.14 COMPONENTS Core REQUIRED) #licensed under the GPL 2.0, as well as the LGPL 3.0
find_package(Qt5 5.14 COMPONENTS Gui REQUIRED) #licensed under the GPL 2.0, as well as the LGPL 3.0
find_package(Qt5 5.14 COMPONENTS Widgets REQUIRED) #licensed under the GPL 2.0, as well as the LGPL 3.0
find_package(Qt5 5.14 COMPONENTS Multimedia REQUIRED) #licensed under the GPL 2.0, as well as the LGPL 3.0
find_package(Qt5 5.14 COMPONENTS Location REQUIRED) #licensed under the GPL 2.0, as well as the LGPL 3.0
find_package(Qt5 5.14 COMPONENTS Qml REQUIRED) #licensed under the GPL 2.0, as well as the LGPL 3.0
find_package(Qt5 5.14 COMPONENTS Quick REQUIRED) #licensed under the GPL 2.0, as well as the LGPL 3.0

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

if(ENABLE_UNITY_BUILD)
	set(CMAKE_UNITY_BUILD ON)
endif()

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
set_source_files_properties(${util_test_SRCS} PROPERTIES UNITY_GROUP "util_test")

target_link_libraries(archimedes ${archimedes_LIBS})

target_precompile_headers(archimedes_test REUSE_FROM archimedes)

target_link_libraries(archimedes_test PUBLIC archimedes)
