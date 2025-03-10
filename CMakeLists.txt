cmake_minimum_required(VERSION 3.16)

project(commentr
    LANGUAGES CXX
    VERSION 1.0.0
    DESCRIPTION "Android code editor."
    HOMEPAGE_URL "https://github.com/przemek83/commentr")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets Network Test)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network Test)
qt_standard_project_setup()

add_definitions(-DQT_WIDGETS_LIB)

if(${ANDROID})
    if(${ANDROID_ABI} STREQUAL "armeabi-v7a")
        set(ARCH_DIR "armeabi-v7a")
    elseif(${ANDROID_ABI} STREQUAL "armeabi")
        set(ARCH_DIR "armeabi")
    elseif(${ANDROID_ABI} STREQUAL "x86")
        set(ARCH_DIR "x86")
    endif()
else()
    if(WIN32)
        set(ARCH_DIR "windows")
    endif()
endif()

set(${PROJECT_NAME}_SOURCES
    src/MainWindow.cpp
    src/MainWindow.h
    src/CodeViewer.cpp
    src/CodeViewer.h
    src/SpellChecker.cpp
    src/SpellChecker.h
    src/EditorTabPage.cpp
    src/EditorTabPage.h
    src/Config.cpp
    src/Config.h
    src/ProxyStyle.cpp
    src/ProxyStyle.h
    src/Highlighters/SQLHighlighter.cpp
    src/Highlighters/SQLHighlighter.h
    src/Highlighters/PythonHighlighter.cpp
    src/Highlighters/PythonHighlighter.h
    src/Highlighters/PhpHighlighter.cpp
    src/Highlighters/PhpHighlighter.h
    src/Highlighters/ObjectiveCHighlighter.cpp
    src/Highlighters/ObjectiveCHighlighter.h
    src/Highlighters/JavaScriptHighlighter.cpp
    src/Highlighters/JavaScriptHighlighter.h
    src/Highlighters/JavaHighlighter.cpp
    src/Highlighters/JavaHighlighter.h
    src/Highlighters/Highlighter.cpp
    src/Highlighters/Highlighter.h
    src/Highlighters/CSharpHighlighter.cpp
    src/Highlighters/CSharpHighlighter.h
    src/Highlighters/CplusPlusHighlighter.cpp
    src/Highlighters/CplusPlusHighlighter.h
    src/Highlighters/CFamilyHighlighter.cpp
    src/Highlighters/CFamilyHighlighter.h
    src/Highlighters/BasicHighlighter.cpp
    src/Highlighters/BasicHighlighter.h
    src/BackButtonHandler.cpp
    src/BackButtonHandler.h
    src/CursorPointer.cpp
    src/CursorPointer.h
    src/EnhancedLineEdit.cpp
    src/EnhancedLineEdit.h
    src/CursorPointerLineEdit.cpp
    src/CursorPointerLineEdit.h
    src/CursorPointerTextEdit.cpp
    src/CursorPointerTextEdit.h
    src/CursorPointerSelector.cpp
    src/CursorPointerSelector.h
    src/Common.cpp
    src/Common.h
    src/FileBrowser/BrowseFilesWidget.cpp
    src/FileBrowser/BrowseFilesWidget.h
    src/ConnectionSetup.cpp
    src/ConnectionSetup.h
    src/FileBrowser/Explorer.cpp
    src/FileBrowser/Explorer.h
    src/FileBrowser/ExplorerLocal.cpp
    src/FileBrowser/ExplorerLocal.h
    src/FileBrowser/ExplorerFtp.cpp
    src/FileBrowser/ExplorerFtp.h
    src/File.cpp
    src/File.h
    src/FtpFileSaver.cpp
    src/FtpFileSaver.h
    src/PanGestureRecognizer.cpp
    src/PanGestureRecognizer.h
)

set(${PROJECT_NAME}_UI
    src/MainWindow.ui
    src/EditorTabPage.ui
    src/FileBrowser/BrowseFilesWidget.ui
    src/ConnectionSetup.ui
)

set(${PROJECT_NAME}_OTHER_FILES
    android/AndroidManifest.xml
    comment.rc
)

qt_add_resources(${PROJECT_NAME}_RESOURCES res/resources.qrc)

if(${ANDROID})
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        src/main.cpp ${${PROJECT_NAME}_SOURCES} ${${PROJECT_NAME}_UI} ${${PROJECT_NAME}_RESOURCES} ${${PROJECT_NAME}_OTHER_FILES}
    )
   set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
                ${CMAKE_CURRENT_SOURCE_DIR}/android)
else()
    add_library (${PROJECT_NAME}-lib STATIC ${${PROJECT_NAME}_SOURCES} ${${PROJECT_NAME}_UI})
    target_link_libraries(${PROJECT_NAME}-lib PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Network)

    add_executable(${PROJECT_NAME} src/main.cpp ${${PROJECT_NAME}_RESOURCES} ${${PROJECT_NAME}_OTHER_FILES})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}-lib Qt6::Widgets)
endif()


include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(${ANDROID})
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Network)
    qt_finalize_executable(${PROJECT_NAME})
endif()

# enable_testing()
# add_subdirectory(tests)
