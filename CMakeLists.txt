cmake_minimum_required(VERSION 3.14)

project(neurus_gstreamer LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the Qt libraries
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets Multimedia MultimediaWidgets Test)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets Multimedia MultimediaWidgets Test REQUIRED)


# Find the GStreamer libraries
find_package(PkgConfig REQUIRED)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0 gstreamer-video-1.0)

file(GLOB_RECURSE SOURCES "*.cpp" "*.h")
add_executable(neurus_gstreamer ${SOURCES}
    videoplayer/source/constructor_videoplayer.cpp
    videoplayer/source/setUI_videoplayer.cpp
    videoplayer/source/controller_videoplayer.cpp
    videostreamer/source/determine_device.cpp
    videostreamer/source/build_pipeline.cpp
    videostreamer/source/constructor_desctructor.cpp
    videostreamer/source/pipeline_controller.cpp
    tests/videostreamertest.h tests/videostreamertest.cpp)

# Link GStreamer libraries
target_include_directories(neurus_gstreamer PUBLIC ${GSTREAMER_INCLUDE_DIRS})

# Link Qt libraries
target_link_libraries(neurus_gstreamer PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Multimedia
    Qt${QT_VERSION_MAJOR}::MultimediaWidgets
    Qt${QT_VERSION_MAJOR}::Test
    ${GSTREAMER_LIBRARIES}
)
target_compile_options(neurus_gstreamer PUBLIC ${GSTREAMER_CFLAGS_OTHER})

include(GNUInstallDirs)
install(TARGETS neurus_gstreamer
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
