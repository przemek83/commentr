cmake_minimum_required(VERSION 3.16)

project(commentr
    LANGUAGES CXX
    VERSION 1.0.0
    DESCRIPTION "Android code editor."
    HOMEPAGE_URL "https://github.com/przemek83/commentr")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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

# Add source files
set(${PROJECT_NAME}_SOURCES
    main.cpp
    MainWindow.cpp
    MainWindow.h
    CodeViewer.cpp
    CodeViewer.h
    SpellChecker.cpp
    SpellChecker.h
    EditorTabPage.cpp
    EditorTabPage.h
    Config.cpp
    Config.h
    ProxyStyle.cpp
    ProxyStyle.h
    Highlighters/SQLHighlighter.cpp
    Highlighters/SQLHighlighter.h
    Highlighters/PythonHighlighter.cpp
    Highlighters/PythonHighlighter.h
    Highlighters/PhpHighlighter.cpp
    Highlighters/PhpHighlighter.h
    Highlighters/ObjectiveCHighlighter.cpp
    Highlighters/ObjectiveCHighlighter.h
    Highlighters/JavaScriptHighlighter.cpp
    Highlighters/JavaScriptHighlighter.h
    Highlighters/JavaHighlighter.cpp
    Highlighters/JavaHighlighter.h
    Highlighters/Highlighter.cpp
    Highlighters/Highlighter.h
    Highlighters/CSharpHighlighter.cpp
    Highlighters/CSharpHighlighter.h
    Highlighters/CplusPlusHighlighter.cpp
    Highlighters/CplusPlusHighlighter.h
    Highlighters/CFamilyHighlighter.cpp
    Highlighters/CFamilyHighlighter.h
    Highlighters/BasicHighlighter.cpp
    Highlighters/BasicHighlighter.h
    BackButtonHandler.cpp
    BackButtonHandler.h
    CursorPointer.cpp
    CursorPointer.h
    EnhancedLineEdit.cpp
    EnhancedLineEdit.h
    CursorPointerLineEdit.cpp
    CursorPointerLineEdit.h
    CursorPointerTextEdit.cpp
    CursorPointerTextEdit.h
    CursorPointerSelector.cpp
    CursorPointerSelector.h
    Common.cpp
    Common.h
    FileBrowser/BrowseFilesWidget.cpp
    FileBrowser/BrowseFilesWidget.h
    ConnectionSetup.cpp
    ConnectionSetup.h
    FileBrowser/Explorer.cpp
    FileBrowser/Explorer.h
    FileBrowser/ExplorerLocal.cpp
    FileBrowser/ExplorerLocal.h
    FileBrowser/ExplorerFtp.cpp
    FileBrowser/ExplorerFtp.h
    File.cpp
    File.h
    FtpFileSaver.cpp
    FtpFileSaver.h
    PanGestureRecognizer.cpp
    PanGestureRecognizer.h
)

# Add UI files
set(FORMS
    MainWindow.ui
    EditorTabPage.ui
    FileBrowser/BrowseFilesWidget.ui
    ConnectionSetup.ui
)

qt_add_resources(${PROJECT_NAME}_RESOURCES Resources/resources.qrc)

# Add other files
set(OTHER_FILES
    android/AndroidManifest.xml
    comment.rc
)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES} ${FORMS} ${OTHER_FILES} ${${PROJECT_NAME}_RESOURCES})

target_link_libraries(${PROJECT_NAME} Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Network)
