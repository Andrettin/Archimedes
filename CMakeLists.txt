cmake_minimum_required(VERSION 3.18.0)

set(CMAKE_CONFIGURATION_TYPES "Debug;RelWithDebInfo" CACHE STRING "" FORCE)

include_directories(
	src
	src/third_party/magic_enum/include
	src/third_party/maskedmousearea
	src/third_party/qcoro
)

set(character_SRCS
	src/character/character_base.cpp
)
source_group(character FILES ${character_SRCS})
set_source_files_properties(${character_SRCS} PROPERTIES UNITY_GROUP "character")

set(database_SRCS
	src/database/basic_data_entry.cpp
	src/database/data_entry.cpp
	src/database/data_entry_history.cpp
	src/database/data_module.cpp
	src/database/data_module_container.cpp
	src/database/data_type_metadata.cpp
	src/database/database.cpp
	src/database/database_util.cpp
	src/database/defines_base.cpp
	src/database/gsml_data.cpp
	src/database/gsml_parser.cpp
	src/database/gsml_property.cpp
	src/database/named_data_entry.cpp
	src/database/predefines.cpp
)
source_group(database FILES ${database_SRCS})
set_source_files_properties(${database_SRCS} PROPERTIES UNITY_GROUP "database")

set(game_SRCS
	src/game/game_rule.cpp
	src/game/game_rule_container.cpp
	src/game/game_rule_group.cpp
)
source_group(game FILES ${game_SRCS})
set_source_files_properties(${game_SRCS} PROPERTIES UNITY_GROUP "game")

set(language_SRCS
	src/language/gendered_name_generator.cpp
	src/language/grammatical_gender.cpp
	src/language/language.cpp
	src/language/name_generator.cpp
	src/language/name_variant.cpp
	src/language/word.cpp
	src/language/word_type.cpp
)
source_group(language FILES ${language_SRCS})
set_source_files_properties(${language_SRCS} PROPERTIES UNITY_GROUP "language")

set(map_SRCS
	src/map/central_cylindrical_map_projection.cpp
	src/map/direction.cpp
	src/map/map_projection.cpp
	src/map/mercator_map_projection.cpp
	src/map/miller_map_projection.cpp
)
source_group(map FILES ${map_SRCS})
set_source_files_properties(${map_SRCS} PROPERTIES UNITY_GROUP "map")

set(script_SRCS
)
source_group(script FILES ${script_SRCS})
set_source_files_properties(${script_SRCS} PROPERTIES UNITY_GROUP "script")

set(time_SRCS
	src/time/calendar.cpp
	src/time/era.cpp
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
	src/util/exception_util.cpp
	src/util/fractional_int.cpp
	src/util/gender.cpp
	src/util/geocircle_util.cpp
	src/util/geocoordinate.cpp
	src/util/geojson_util.cpp
	src/util/geopath_util.cpp
	src/util/geopolygon_util.cpp
	src/util/geoshape_util.cpp
	src/util/image_provider_base.cpp
	src/util/image_util.cpp
	src/util/log_level.cpp
	src/util/log_util.cpp
	src/util/number_util.cpp
	src/util/point_container.cpp
	src/util/point_util.cpp
	src/util/qgeocoordinate_util.cpp
	src/util/random.cpp
	src/util/rect_util.cpp
	src/util/string_conversion_util.cpp
	src/util/string_util.cpp
	src/util/text_processor_base.cpp
	src/util/time_util.cpp
)
source_group(util FILES ${util_SRCS})
set_source_files_properties(${util_SRCS} PROPERTIES UNITY_GROUP "util")

set(maskedmousearea_SRCS
	src/third_party/maskedmousearea/maskedmousearea.cpp
)
source_group(maskedmousearea FILES ${maskedmousearea_SRCS})
set_source_files_properties(${maskedmousearea_SRCS} PROPERTIES UNITY_GROUP "maskedmousearea")

set(archimedes_SRCS
	${character_SRCS}
	${database_SRCS}
	${game_SRCS}
	${language_SRCS}
	${map_SRCS}
	${script_SRCS}
	${time_SRCS}
	${util_SRCS}
	${maskedmousearea_SRCS}
)

set(character_HDRS
	src/character/character_base.h
)
source_group(character FILES ${character_HDRS})

set(database_HDRS
	src/database/basic_data_entry.h
	src/database/data_entry.h
	src/database/data_entry_history.h
	src/database/data_module.h
	src/database/data_module_container.h
	src/database/data_type.h
	src/database/data_type_metadata.h
	src/database/database.h
	src/database/database_util.h
	src/database/defines_base.h
	src/database/enum_data_type.h
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

set(game_HDRS
	src/game/game_rule.h
	src/game/game_rule_container.h
	src/game/game_rule_group.h
	src/game/game_rules_base.h
)
source_group(game FILES ${game_HDRS})

set(language_HDRS
	src/language/gendered_name_generator.h
	src/language/grammatical_gender.h
	src/language/language.h
	src/language/language_family.h
	src/language/name_generator.h
	src/language/name_variant.h
	src/language/word.h
	src/language/word_type.h
)
source_group(language FILES ${language_HDRS})

set(map_HDRS
	src/map/central_cylindrical_map_projection.h
	src/map/direction.h
	src/map/equirectangular_map_projection.h
	src/map/map_projection.h
	src/map/mercator_map_projection.h
	src/map/miller_map_projection.h
)
source_group(map FILES ${map_HDRS})

set(script_HDRS
	src/script/special_target_type.h
	src/script/target_variant.h
)
source_group(script FILES ${script_HDRS})

set(script_condition_HDRS
	src/script/condition/and_condition_base.h
	src/script/condition/birth_year_condition.h
	src/script/condition/condition_base.h
	src/script/condition/condition_base.ipp
	src/script/condition/gender_condition.h
	src/script/condition/not_condition.h
	src/script/condition/numerical_condition.h
	src/script/condition/or_condition.h
	src/script/condition/saved_scope_condition.h
	src/script/condition/scope_condition.h
	src/script/condition/scope_condition_base.h
	src/script/condition/scripted_condition_base.h
	src/script/condition/tooltip_condition.h
)
source_group(script\\condition FILES ${script_condition_HDRS})

set(script_effect_HDRS
	src/script/effect/effect_base.h
)
source_group(script\\effect FILES ${script_effect_HDRS})

set(script_modifier_effect_HDRS
	src/script/modifier_effect/modifier_effect_base.h
)
source_group(script\\modifier_effect FILES ${script_modifier_effect_HDRS})

set(time_HDRS
	src/time/calendar.h
	src/time/day_of_the_week.h
	src/time/era.h
	src/time/month.h
	src/time/timeline.h
)
source_group(time FILES ${time_HDRS})

set(util_HDRS
	src/util/aggregate_exception.h
	src/util/angle_util.h
	src/util/assert_util.h
	src/util/astronomy_util.h
	src/util/centesimal_int.h
	src/util/char_util.h
	src/util/color_container.h
	src/util/colorization_type.h
	src/util/container_random_util.h
	src/util/container_util.h
	src/util/date_util.h
	src/util/decimal_int.h
	src/util/decimillesimal_int.h
	src/util/dice.h
	src/util/empty_image_provider.h
	src/util/enum_converter.h
	src/util/enum_util.h
	src/util/exception_util.h
	src/util/fractional_int.h
	src/util/gender.h
	src/util/geocircle_util.h
	src/util/geocoordinate.h
	src/util/geojson_util.h
	src/util/geopath_util.h
	src/util/geopolygon_util.h
	src/util/georectangle.h
	src/util/geoshape_util.h
	src/util/image_provider_base.h
	src/util/image_util.h
	src/util/list_util.h
	src/util/locale_util.h
	src/util/log_level.h
	src/util/log_output_handler.h
	src/util/log_util.h
	src/util/map_util.h
	src/util/millesimal_int.h
	src/util/number_util.h
	src/util/path_util.h
	src/util/point_container.h
	src/util/point_operators.h
	src/util/point_util.h
	src/util/qgeocoordinate_util.h
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
	src/util/time_util.h
	src/util/type_traits.h
	src/util/vector_random_util.h
	src/util/vector_util.h
)
source_group(util FILES ${util_HDRS})

set(maskedmousearea_HDRS
	src/third_party/maskedmousearea/maskedmousearea.h
)
source_group(maskedmousearea FILES ${maskedmousearea_HDRS})

set(archimedes_HDRS
	${character_HDRS}
	${database_HDRS}
	${game_HDRS}
	${language_HDRS}
	${map_HDRS}
	${script_HDRS}
	${script_condition_HDRS}
	${script_effect_HDRS}
	${script_modifier_effect_HDRS}
	${time_HDRS}
	${util_HDRS}
	${maskedmousearea_HDRS}
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

#Qt6 modules
#different modules have different licenses, make sure all modules used here are compatible with the MIT license
set(CMAKE_AUTOMOC ON)
find_package(Qt6 6.8 COMPONENTS Core REQUIRED) #licensed under the GPL 2.0, as well as the LGPL 3.0
find_package(Qt6 6.8 COMPONENTS Gui REQUIRED) #licensed under the GPL 2.0, as well as the LGPL 3.0
find_package(Qt6 6.8 COMPONENTS Widgets REQUIRED) #licensed under the GPL 2.0, as well as the LGPL 3.0
find_package(Qt6 6.8 COMPONENTS Multimedia REQUIRED) #licensed under the GPL 2.0, as well as the LGPL 3.0
find_package(Qt6 6.8 COMPONENTS Location REQUIRED) #licensed under the GPL 2.0, as well as the LGPL 3.0
find_package(Qt6 6.8 COMPONENTS Qml REQUIRED) #licensed under the GPL 2.0, as well as the LGPL 3.0
find_package(Qt6 6.8 COMPONENTS Quick REQUIRED) #licensed under the GPL 2.0, as well as the LGPL 3.0

add_subdirectory(src/third_party/qcoro)

set(QT_LIBRARIES
	Qt6::Core
	Qt6::Gui
	Qt6::Widgets
	Qt6::Multimedia
	Qt6::Location
	Qt6::Qml
	Qt6::Quick
)

if(WITH_GEOJSON)
	add_definitions(-DUSE_GEOJSON)
	set(QT_LIBRARIES ${QT_LIBRARIES} Qt6::LocationPrivate)
endif()

if(MSVC)
	add_definitions(/FI"archimedes.h")
else()
	#GCC/Clang
	add_definitions(-include archimedes.h)
endif()

set(archimedes_LIBS
	${QT_LIBRARIES}
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(ENABLE_UNITY_BUILD)
	set(CMAKE_UNITY_BUILD ON)
endif()

if(MSVC)
	add_compile_options(/W4 /w44800 /wd4244 /wd4458 /MP)
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

target_link_libraries(archimedes ${archimedes_LIBS} QCoro6Core QCoro6Network QCoro6Qml)

target_precompile_headers(archimedes_test REUSE_FROM archimedes)

target_link_libraries(archimedes_test PUBLIC archimedes)
