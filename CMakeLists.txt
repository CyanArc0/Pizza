cmake_minimum_required(VERSION 3.16)

project(Pizza VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

include(disable-Exceptions-RTTI.cmake)

qt_add_executable(appPizza
    main.cpp
    resources/Pizza.rc
)

qt_add_qml_module(appPizza
    URI Pizza
    VERSION 1.0
    QML_FILES
    UI.qml
        RESOURCES android/AndroidManifest.xml android/build.gradle android/res/values/libs.xml
        SOURCES Bridge.hpp
        RESOURCES resources/resources.qrc
        SOURCES core.cpp zip.hpp
)

set_property(TARGET appPizza APPEND PROPERTY
    QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appPizza PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appPizza
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appPizza
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appPizza
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
