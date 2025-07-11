cmake_minimum_required(VERSION 3.16)

project(calculator_2 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu/qt6")
set(QML_IMPORT_PATH "/usr/lib/x86_64-linux-gnu/qt6/qml")

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick QuickControls2 QuickTemplates2)

qt_standard_project_setup()

qt_add_executable(appcalculator_2
    resources.qrc
    main.cpp
)


qt_add_qml_module(appcalculator_2
    URI calculator_2
    VERSION 1.0
    QML_FILES
        Main.qml
        MainMenu.qml
        HiddenMenu.qml
        CalculatorButton.qml


)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appcalculator_2 PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appcalculator_2
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appcalculator_2
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appcalculator_2
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
