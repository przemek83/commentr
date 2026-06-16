SET(PROJECT_TESTS ${PROJECT_NAME}-tests)

set(${PROJECT_TESTS}_SOURCES
    Tests.cpp
    JavaHighlighterTest.h
    JavaHighlighterTest.cpp
    PythonHighlighterTest.h
    PythonHighlighterTest.cpp
    HighlighterTestHelpers.h
    HighlighterTestHelpers.cpp
    CHighlighterTest.h
    CHighlighterTest.cpp
    CplusPlusHighlighterTest.h
    CplusPlusHighlighterTest.cpp
    ObjectiveCHighlighterTest.h
    ObjectiveCHighlighterTest.cpp
    PhpHighlighterTest.h
    PhpHighlighterTest.cpp
    JavaScriptHighlighterTest.h
    JavaScriptHighlighterTest.cpp
    CSharpHighlighterTest.h
    CSharpHighlighterTest.cpp
    SQLHighlighterTest.h
    SQLHighlighterTest.cpp
    SpellCheckerTest.h
    SpellCheckerTest.cpp
    HighlighterSpellCheckTest.h
    HighlighterSpellCheckTest.cpp
    VisualBasicHighlighterTest.h
    VisualBasicHighlighterTest.cpp
    FileTest.h
    FileTest.cpp
    TestFileExplorer.h
    TestFileExplorer.cpp
)

qt_add_resources(${PROJECT_TESTS}_SOURCES ../res/resources.qrc)

add_executable(${PROJECT_TESTS} ${${PROJECT_TESTS}_SOURCES})

target_include_directories(${PROJECT_TESTS} PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(${PROJECT_TESTS} PRIVATE ${PROJECT_NAME}-lib Qt6::Core Qt6::Widgets Qt6::Test)

add_test(NAME ${PROJECT_TESTS} COMMAND ${PROJECT_TESTS})

set_property(TEST ${PROJECT_TESTS} PROPERTY FAIL_REGULAR_EXPRESSION "FAIL!")
