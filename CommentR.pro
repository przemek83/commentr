#-------------------------------------------------
#
# Project created by QtCreator 2013-10-29T10:26:42
#
#-------------------------------------------------

CONFIG += c++11

QT += network core gui widgets #androidextras opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

android {
    equals(ANDROID_TARGET_ARCH, armeabi-v7a) {
        ARCH_DIR=armeabi-v7a
    }
    equals(ANDROID_TARGET_ARCH, armeabi) {
        ARCH_DIR=armeabi
    }
    equals(ANDROID_TARGET_ARCH, x86)  {
        ARCH_DIR=x86
    }
}

win32: {
   ARCH_DIR=windows
}

#FTP
FTP_ROOT = D:/Programowanie/CommentR/qtftp/
INCLUDEPATH += $${FTP_ROOT}include
LIBS += -L$${FTP_ROOT}lib/$$ARCH_DIR
CONFIG(debug, debug|release) {
    LIBS += -lqtftp
    ANDROID_EXTRA_LIBS = $${FTP_ROOT}lib/$$ARCH_DIR/debug/libqtftp.so
} else {
    LIBS += -lqtftp
    ANDROID_EXTRA_LIBS = $${FTP_ROOT}lib/$$ARCH_DIR/libqtftp.so
}

TARGET = CommentR
TEMPLATE = app

RC_FILE = comment.rc

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

SOURCES += main.cpp\
    MainWindow.cpp \
    CodeViewer.cpp \
    SpellChecker.cpp \
    EditorTabPage.cpp \
    Config.cpp \
    ProxyStyle.cpp \
    Highlighters/SQLHighlighter.cpp \
    Highlighters/PythonHighlighter.cpp \
    Highlighters/PhpHighlighter.cpp \
    Highlighters/ObjectiveCHighlighter.cpp \
    Highlighters/JavaScriptHighlighter.cpp \
    Highlighters/JavaHighlighter.cpp \
    Highlighters/Highlighter.cpp \
    Highlighters/CSharpHighlighter.cpp \
    Highlighters/CplusPlusHighlighter.cpp \
    Highlighters/CFamilyHighlighter.cpp \
    Highlighters/BasicHighlighter.cpp \
    BackButtonHandler.cpp \
    CursorPointer.cpp \
    EnhancedLineEdit.cpp \
    CursorPointerLineEdit.cpp \
    CursorPointerTextEdit.cpp \
    CursorPointerSelector.cpp \
    Common.cpp \
    FileBrowser/BrowseFilesWidget.cpp \
    ConnectionSetup.cpp \
    FileBrowser/Explorer.cpp \
    FileBrowser/ExplorerLocal.cpp \
    FileBrowser/ExplorerFtp.cpp \
    File.cpp \
    FtpFileSaver.cpp \
    PanGestureRecognizer.cpp

HEADERS  += MainWindow.h \
    CodeViewer.h \
    SpellChecker.h \
    EditorTabPage.h \
    Config.h \
    ProxyStyle.h \
    Highlighters/SQLHighlighter.h \
    Highlighters/PythonHighlighter.h \
    Highlighters/PhpHighlighter.h \
    Highlighters/ObjectiveCHighlighter.h \
    Highlighters/JavaScriptHighlighter.h \
    Highlighters/JavaHighlighter.h \
    Highlighters/Highlighter.h \
    Highlighters/CSharpHighlighter.h \
    Highlighters/CplusPlusHighlighter.h \
    Highlighters/CFamilyHighlighter.h \
    Highlighters/BasicHighlighter.h \
    Defines.h \
    BackButtonHandler.h \
    CursorPointer.h \
    EnhancedLineEdit.h \
    CursorPointerLineEdit.h \
    CursorPointerTextEdit.h \
    CursorPointerSelector.h \
    Common.h \
    FileBrowser/BrowseFilesWidget.h \
    ConnectionSetup.h \
    FileBrowser/Explorer.h \
    FileBrowser/ExplorerLocal.h \
    FileBrowser/ExplorerFtp.h \
    File.h \
    FtpFileSaver.h \
    PanGestureRecognizer.h

FORMS    += MainWindow.ui \
    EditorTabPage.ui \
    FileBrowser/BrowseFilesWidget.ui \
    ConnectionSetup.ui

RESOURCES += \
    Resources/resources.qrc

OTHER_FILES += \
    android/AndroidManifest.xml \
    comment.rc
