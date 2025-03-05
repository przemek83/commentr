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
set(SOURCES
    main.cpp
    MainWindow.cpp
    CodeViewer.cpp
    SpellChecker.cpp
    EditorTabPage.cpp
    Config.cpp
    ProxyStyle.cpp
    Highlighters/SQLHighlighter.cpp
    Highlighters/PythonHighlighter.cpp
    Highlighters/PhpHighlighter.cpp
    Highlighters/ObjectiveCHighlighter.cpp
    Highlighters/JavaScriptHighlighter.cpp
    Highlighters/JavaHighlighter.cpp
    Highlighters/Highlighter.cpp
    Highlighters/CSharpHighlighter.cpp
    Highlighters/CplusPlusHighlighter.cpp
    Highlighters/CFamilyHighlighter.cpp
    Highlighters/BasicHighlighter.cpp
    BackButtonHandler.cpp
    CursorPointer.cpp
    EnhancedLineEdit.cpp
    CursorPointerLineEdit.cpp
    CursorPointerTextEdit.cpp
    CursorPointerSelector.cpp
    Common.cpp
    FileBrowser/BrowseFilesWidget.cpp
    ConnectionSetup.cpp
    FileBrowser/Explorer.cpp
    FileBrowser/ExplorerLocal.cpp
    FileBrowser/ExplorerFtp.cpp
    File.cpp
    FtpFileSaver.cpp
    PanGestureRecognizer.cpp
)

# Add header files
set(HEADERS
    MainWindow.h
    CodeViewer.h
    SpellChecker.h
    EditorTabPage.h
    Config.h
    ProxyStyle.h
    Highlighters/SQLHighlighter.h
    Highlighters/PythonHighlighter.h
    Highlighters/PhpHighlighter.h
    Highlighters/ObjectiveCHighlighter.h
    Highlighters/JavaScriptHighlighter.h
    Highlighters/JavaHighlighter.h
    Highlighters/Highlighter.h
    Highlighters/CSharpHighlighter.h
    Highlighters/CplusPlusHighlighter.h
    Highlighters/CFamilyHighlighter.h
    Highlighters/BasicHighlighter.h
    Defines.h
    BackButtonHandler.h
    CursorPointer.h
    EnhancedLineEdit.h
    CursorPointerLineEdit.h
    CursorPointerTextEdit.h
    CursorPointerSelector.h
    Common.h
    FileBrowser/BrowseFilesWidget.h
    ConnectionSetup.h
    FileBrowser/Explorer.h
    FileBrowser/ExplorerLocal.h
    FileBrowser/ExplorerFtp.h
    File.h
    FtpFileSaver.h
    PanGestureRecognizer.h
)

# Add UI files
set(FORMS
    MainWindow.ui
    EditorTabPage.ui
    FileBrowser/BrowseFilesWidget.ui
    ConnectionSetup.ui
)

# Add resource files
set(RESOURCES
    Resources/resources.qrc
)

# Add other files
set(OTHER_FILES
    android/AndroidManifest.xml
    comment.rc
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${FORMS} ${RESOURCES} ${OTHER_FILES})

target_link_libraries(${PROJECT_NAME} Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Network)
