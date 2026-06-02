#include "JavaScriptHighlighter.h"

JavaScriptHighlighter::JavaScriptHighlighter(const SpellChecker& spellChecker,
                                             QObject* parent)
    : Highlighter(spellChecker, parent)
{
    singleLineCommentRule_.format_.setForeground(Qt::red);
    singleLineCommentRule_.startPattern_ =
        QRegularExpression(QStringLiteral("//[^\n]*"));

    multiLineCommentRule_.startPattern_ =
        QRegularExpression(QStringLiteral("/\\*"));
    multiLineCommentRule_.endPattern_ =
        QRegularExpression(QStringLiteral("\\*/"));
    multiLineCommentRule_.format_.setForeground(Qt::red);
}

void JavaScriptHighlighter::initRules()
{
    HighlightingRule rule;
    QTextCharFormat functionFormat;
    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.startPattern_ =
        QRegularExpression(QStringLiteral(R"(\b[A-Za-z0-9_]+\s*(?=\())"));
    rule.format_ = functionFormat;
    highlightingRules_.append(rule);

    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;

    keywordPatterns
        << QStringLiteral("\\break\\b") << QStringLiteral("\\bcase\\b")
        << QStringLiteral("\\bcatch\\b") << QStringLiteral("\\bcontinue\\b")
        << QStringLiteral("\\bdebugger\\b") << QStringLiteral("\\bdefault\\b")
        << QStringLiteral("\\bdelete\\b") << QStringLiteral("\\bdo\\b")
        << QStringLiteral("\\belse\\b") << QStringLiteral("\\bfinally\\b")
        << QStringLiteral("\\bfor\\b") << QStringLiteral("\\bfunction\\b")
        << QStringLiteral("\\bif\\b") << QStringLiteral("\\bin\\b")
        << QStringLiteral("\\binstanceof\\b") << QStringLiteral("\\bnew\\b")
        << QStringLiteral("\\breturn\\b") << QStringLiteral("\\bswitch\\b")
        << QStringLiteral("\\bthis\\b") << QStringLiteral("\\bthrow\\b")
        << QStringLiteral("\\btry\\b") << QStringLiteral("\\btypeof\\b")
        << QStringLiteral("\\bvar\\b") << QStringLiteral("\\bvoid\\b")
        << QStringLiteral("\\bwhile\\b") << QStringLiteral("\\bwith\\b")
        << QStringLiteral("\\bconst\\b") << QStringLiteral("\\benum\\b")
        << QStringLiteral("\\bexport\\b") << QStringLiteral("\\bextends\\b")
        << QStringLiteral("\\bimport\\b") << QStringLiteral("\\bsuper\\b")
        << QStringLiteral("\\babstract\\b") << QStringLiteral("\\bboolean\\b")
        << QStringLiteral("\\bbyte\\b") << QStringLiteral("\\bchar\\b")
        << QStringLiteral("\\bclass\\b") << QStringLiteral("\\bdouble\\b")
        << QStringLiteral("\\bfinal\\b") << QStringLiteral("\\bfloat\\b")
        << QStringLiteral("\\bgoto\\b") << QStringLiteral("\\bimplements\\b")
        << QStringLiteral("\\bint\\b") << QStringLiteral("\\binterface\\b")
        << QStringLiteral("\\blong\\b") << QStringLiteral("\\bnative\\b")
        << QStringLiteral("\\bpackage\\b") << QStringLiteral("\\bprivate\\b")
        << QStringLiteral("\\bprotected\\b") << QStringLiteral("\\bpublic\\b")
        << QStringLiteral("\\bshort\\b") << QStringLiteral("\\bstatic\\b")
        << QStringLiteral("\\bsynchronized\\b")
        << QStringLiteral("\\bthrows\\b") << QStringLiteral("\\btransient\\b")
        << QStringLiteral("\\blet\\b") << QStringLiteral("\\byield\\b");

    for (const QString& pattern : keywordPatterns)
    {
        rule.startPattern_ = QRegularExpression(pattern);
        rule.format_ = keywordFormat;
        highlightingRules_.append(rule);
    }

    QTextCharFormat quotationFormat;
    quotationFormat.setForeground(Qt::darkGreen);
    rule.startPattern_ =
        QRegularExpression(QStringLiteral(R"(("([^"]|\\")*"|'([^']|\\')*'))"));
    rule.format_ = quotationFormat;
    highlightingRules_.append(rule);
}

void JavaScriptHighlighter::commentBlock(const QString& text)
{
    singleLineComment(text, singleLineCommentRule_);
    multiLineComment(text, multiLineCommentRule_);
}
