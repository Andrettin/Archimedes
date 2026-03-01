# SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
# SPDX-FileCopyrightText: 2022 Jonah Brüchert <jbb@kaidan.im>
#
# SPDX-License-Identifier: MIT

add_qcoro_library(
    NAME Qml
    INCLUDEDIR Qml
    SOURCES
        qcoroqmltask.cpp
        qcoroqml.cpp
    CAMELCASE_HEADERS
        QCoroQmlTask
        QCoroQml
    QT_LINK_LIBRARIES
        PUBLIC Core Qml
    QCORO_LINK_LIBRARIES
        PUBLIC Coro
)

if (${QT_VERSION_MAJOR} STREQUAL "6")
    if (TARGET Qt::QmlPrivate)
        target_link_libraries(QCoro${QT_VERSION_MAJOR}Qml PRIVATE Qt::QmlPrivate)
    else()
        # Homebrew Qt ships private headers in versioned subdirectories inside frameworks.
        # Add all Qt framework private header dirs so #include <private/foo_p.h> and
        # #include <QtModule/private/foo_p.h> both resolve correctly.
        set(_qt_lib_dir "${Qt6_DIR}/../..")
        file(GLOB _qt_frameworks "${_qt_lib_dir}/*.framework")
        set(_private_include_dirs)
        foreach(_fw ${_qt_frameworks})
            get_filename_component(_fw_name ${_fw} NAME_WE)
            set(_versioned "${_fw}/Headers/${Qt6_VERSION}/${_fw_name}")
            if (EXISTS "${_versioned}/private")
                list(APPEND _private_include_dirs "${_versioned}")
            endif()
            set(_versioned_parent "${_fw}/Headers/${Qt6_VERSION}")
            if (EXISTS "${_versioned_parent}")
                list(APPEND _private_include_dirs "${_versioned_parent}")
            endif()
        endforeach()
        if (_private_include_dirs)
            target_include_directories(QCoro${QT_VERSION_MAJOR}Qml PRIVATE ${_private_include_dirs})
        endif()
    endif()
endif()
