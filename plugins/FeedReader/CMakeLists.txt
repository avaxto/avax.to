################################################################################
# CMakeLists.txt - FeedReader Plugin                                          #
# Copyright (C) 2018, Retroshare team <retroshare.team@gmailcom>               #
#                                                                              #
# This program is free software: you can redistribute it and/or modify         #
# it under the terms of the GNU Affero General Public License as               #
# published by the Free Software Foundation, either version 3 of the           #
# License, or (at your option) any later version.                              #
#                                                                              #
# This program is distributed in the hope that it will be useful,              #
# but WITHOUT ANY WARRANTY; without even the implied warranty of               #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                #
# GNU Lesser General Public License for more details.                          #
#                                                                              #
# You should have received a copy of the GNU Lesser General Public License     #
# along with this program.  If not, see <https://www.gnu.org/licenses/>.       #
################################################################################

cmake_minimum_required(VERSION 3.16)

project(FeedReader)

# Find required packages
find_package(Qt5 REQUIRED COMPONENTS Core Widgets)

# Find system libraries
find_package(PkgConfig REQUIRED)
pkg_check_modules(CURL REQUIRED libcurl)
pkg_check_modules(LIBXML2 REQUIRED libxml-2.0)
pkg_check_modules(LIBXSLT REQUIRED libxslt)

# Set output directory
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)

# Source files
set(SOURCES
    FeedReaderPlugin.cpp
    services/p3FeedReader.cc
    services/p3FeedReaderThread.cc
    services/rsFeedReaderItems.cc
    gui/FeedReaderDialog.cpp
    gui/FeedReaderMessageWidget.cpp
    gui/AddFeedDialog.cpp
    gui/PreviewFeedDialog.cpp
    gui/FeedReaderNotify.cpp
    gui/FeedReaderConfig.cpp
    gui/FeedReaderStringDefs.cpp
    gui/FeedReaderFeedNotify.cpp
    gui/FeedReaderUserNotify.cpp
    gui/FeedReaderFeedItem.cpp
    gui/FeedTreeWidget.cpp
    util/CURLWrapper.cpp
    util/XMLWrapper.cpp
    util/HTMLWrapper.cpp
    util/XPathWrapper.cpp
)

# Header files
set(HEADERS
    FeedReaderPlugin.h
    interface/rsFeedReader.h
    services/p3FeedReader.h
    services/p3FeedReaderThread.h
    services/rsFeedReaderItems.h
    gui/FeedReaderDialog.h
    gui/FeedReaderMessageWidget.h
    gui/AddFeedDialog.h
    gui/PreviewFeedDialog.h
    gui/FeedReaderNotify.h
    gui/FeedReaderConfig.h
    gui/FeedReaderStringDefs.h
    gui/FeedReaderFeedNotify.h
    gui/FeedReaderUserNotify.h
    gui/FeedReaderFeedItem.h
    gui/FeedTreeWidget.h
    util/CURLWrapper.h
    util/XMLWrapper.h
    util/HTMLWrapper.h
    util/XPathWrapper.h
)

# UI files
set(FORMS
    gui/FeedReaderDialog.ui
    gui/FeedReaderMessageWidget.ui
    gui/AddFeedDialog.ui
    gui/PreviewFeedDialog.ui
    gui/FeedReaderConfig.ui
    gui/FeedReaderFeedItem.ui
)

# Resource files
set(RESOURCES
    gui/FeedReader_images.qrc
    lang/FeedReader_lang.qrc
    qss/FeedReader_qss.qrc
)

# Translation files
set(TRANSLATIONS
    lang/FeedReader_ca_ES.ts
    lang/FeedReader_cs.ts
    lang/FeedReader_da.ts
    lang/FeedReader_de.ts
    lang/FeedReader_el.ts
    lang/FeedReader_en.ts
    lang/FeedReader_es.ts
    lang/FeedReader_fi.ts
    lang/FeedReader_fr.ts
    lang/FeedReader_hu.ts
    lang/FeedReader_it.ts
    lang/FeedReader_ja_JP.ts
    lang/FeedReader_ko.ts
    lang/FeedReader_nl.ts
    lang/FeedReader_pl.ts
    lang/FeedReader_ru.ts
    lang/FeedReader_sv.ts
    lang/FeedReader_tr.ts
    lang/FeedReader_zh_CN.ts
)

# Enable Qt MOC, UIC, and RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Create the library
add_library(FeedReader SHARED ${SOURCES} ${HEADERS} ${FORMS} ${RESOURCES})

# Set library properties
set_target_properties(FeedReader PROPERTIES
    VERSION 1.0
    SOVERSION 1
    OUTPUT_NAME FeedReader
)

# Include directories
target_include_directories(FeedReader PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../rapidjson-1.1.0
    ${CURL_INCLUDE_DIRS}
    ${LIBXML2_INCLUDE_DIRS}
    ${LIBXSLT_INCLUDE_DIRS}
)

# Link Qt libraries
target_link_libraries(FeedReader
    Qt5::Core
    Qt5::Widgets
    ${CURL_LIBRARIES}
    ${LIBXML2_LIBRARIES}
    ${LIBXSLT_LIBRARIES}
)

# Platform-specific settings
if(WIN32)
    target_compile_definitions(FeedReader PRIVATE
        CURL_STATICLIB
        LIBXML_STATIC
        LIBXSLT_STATIC
        LIBEXSLT_STATIC
    )
    
    # Windows-specific libraries
    target_link_libraries(FeedReader
        ws2_32
        wldap32
        ssl
        crypto
        gdi32
        crypt32
        z
    )
    
    # MSYS2 support
    if(DEFINED ENV{PREFIX_MSYS2})
        target_include_directories(FeedReader PRIVATE
            $ENV{PREFIX_MSYS2}/include/libxml2
        )
    endif()
    
elseif(APPLE)
    target_compile_definitions(FeedReader PRIVATE
        CURL_STATICLIB
        LIBXML_STATIC
        LIBXSLT_STATIC
        LIBEXSLT_STATIC
    )
    
    # macOS-specific libraries
    target_link_libraries(FeedReader
        crypto
    )
    
elseif(CMAKE_SYSTEM_NAME MATCHES "OpenBSD")
    target_include_directories(FeedReader PRIVATE
        /usr/local/include/libxml2
    )
    
elseif(CMAKE_SYSTEM_NAME MATCHES "Haiku")
    # Haiku-specific settings would go here if needed
    pass()
endif()

# Compiler flags from pkg-config
target_compile_options(FeedReader PRIVATE
    ${CURL_CFLAGS_OTHER}
    ${LIBXML2_CFLAGS_OTHER}
    ${LIBXSLT_CFLAGS_OTHER}
)

# Link directories
target_link_directories(FeedReader PRIVATE
    ${CURL_LIBRARY_DIRS}
    ${LIBXML2_LIBRARY_DIRS}
    ${LIBXSLT_LIBRARY_DIRS}
)

# Install target
install(TARGETS FeedReader
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
)
