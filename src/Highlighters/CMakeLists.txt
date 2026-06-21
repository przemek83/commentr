project(highlighters)

set(SOURCES
    VisualBasicHighlighter.cpp
    VisualBasicHighlighter.h
    CFamilyHighlighter.cpp
    CFamilyHighlighter.h
    CPlusPlusHighlighter.cpp
    CPlusPlusHighlighter.h
    CSharpHighlighter.cpp
    CSharpHighlighter.h
    CHighlighter.cpp
    CHighlighter.h
    Highlighter.cpp
    Highlighter.h
    JavaHighlighter.cpp
    JavaHighlighter.h
    JavaScriptHighlighter.cpp
    JavaScriptHighlighter.h
    ObjectiveCHighlighter.cpp
    ObjectiveCHighlighter.h
    PhpHighlighter.cpp
    PhpHighlighter.h
    PythonHighlighter.cpp
    PythonHighlighter.h
    SQLHighlighter.cpp
    SQLHighlighter.h
)

ADD_LIBRARY(${PROJECT_NAME} STATIC ${SOURCES})

target_link_libraries(${PROJECT_NAME} Qt6::Widgets)
