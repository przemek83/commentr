project(highlighters)

set(SOURCES
    BasicHighlighter.cpp
    BasicHighlighter.h
    CFamilyHighlighter.cpp
    CFamilyHighlighter.h
    CplusPlusHighlighter.cpp
    CplusPlusHighlighter.h
    CSharpHighlighter.cpp
    CSharpHighlighter.h
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
