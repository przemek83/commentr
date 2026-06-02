#include "PhpHighlighter.h"

PhpHighlighter::PhpHighlighter(const SpellChecker& spellChecker,
                               QObject* parent)
    : Highlighter(spellChecker, parent)
{
    singleLineCommentRule_.format_.setForeground(Qt::red);
    singleLineCommentRule_.startPattern_ =
        QRegularExpression(QStringLiteral("(//[^\n]*|#[^\n]*)"));

    multiLineCommentRule_.startPattern_ = QRegularExpression(QStringLiteral("/\\*"));
    multiLineCommentRule_.endPattern_ = QRegularExpression(QStringLiteral("\\*/"));
    multiLineCommentRule_.format_.setForeground(Qt::red);
}

void PhpHighlighter::initRules()
{
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;

    keywordPatterns << QStringLiteral("\\__halt_compiler\\b") << QStringLiteral("\\babstract\\b") << QStringLiteral("\\band\\b")
                    << QStringLiteral("\\barray\\b") << QStringLiteral("\\bas\\b") << QStringLiteral("\\bbreak\\b")
                    << QStringLiteral("\\bcallable\\b") << QStringLiteral("\\bcase\\b") << QStringLiteral("\\bcatch\\b")
                    << QStringLiteral("\\bclass\\b") << QStringLiteral("\\bclone\\b") << QStringLiteral("\\bconst\\b")
                    << QStringLiteral("\\bcontinue\\b") << QStringLiteral("\\bdeclare\\b") << QStringLiteral("\\bdefault\\b")
                    << QStringLiteral("\\bdie\\b") << QStringLiteral("\\bdo\\b") << QStringLiteral("\\becho\\b") << QStringLiteral("\\belse\\b")
                    << QStringLiteral("\\belseif\\b") << QStringLiteral("\\bempty\\b") << QStringLiteral("\\benddeclare\\b")
                    << QStringLiteral("\\bendfor\\b") << QStringLiteral("\\bendforeach\\b") << QStringLiteral("\\bendif\\b")
                    << QStringLiteral("\\bendswitch\\b") << QStringLiteral("\\bendwhile\\b") << QStringLiteral("\\beval\\b")
                    << QStringLiteral("\\bexit\\b") << QStringLiteral("\\bextends\\b") << QStringLiteral("\\bfinal\\b")
                    << QStringLiteral("\\bfor\\b") << QStringLiteral("\\bforeach\\b") << QStringLiteral("\\bfunction\\b")
                    << QStringLiteral("\\bglobal\\b") << QStringLiteral("\\bgoto\\b") << QStringLiteral("\\bif\\b")
                    << QStringLiteral("\\bimplements\\b") << QStringLiteral("\\binclude\\b")
                    << QStringLiteral("\\binclude_once\\b") << QStringLiteral("\\binstanceof\\b")
                    << QStringLiteral("\\binsteadof\\b") << QStringLiteral("\\binterface\\b") << QStringLiteral("\\bisset\\b")
                    << QStringLiteral("\\blist\\b") << QStringLiteral("\\bnamespace\\b") << QStringLiteral("\\bnew\\b")
                    << QStringLiteral("\\bor\\b") << QStringLiteral("\\bprint\\b") << QStringLiteral("\\bprivate\\b")
                    << QStringLiteral("\\bprotected\\b") << QStringLiteral("\\bpublic\\b") << QStringLiteral("\\brequire\\b")
                    << QStringLiteral("\\brequire_once\\b") << QStringLiteral("\\breturn\\b") << QStringLiteral("\\bstatic\\b")
                    << QStringLiteral("\\bswitch\\b") << QStringLiteral("\\bthrow\\b") << QStringLiteral("\\btrait\\b")
                    << QStringLiteral("\\btry\\b") << QStringLiteral("\\bunset\\b") << QStringLiteral("\\buse\\b")
                    << QStringLiteral("\\bvar\\b") << QStringLiteral("\\bwhile\\b") << QStringLiteral("\\bxor\\b")
                    << QStringLiteral("\\byield\\b");

    keywordPatterns << QStringLiteral("\\b__CLASS__\\b") << QStringLiteral("\\b__DIR__\\b") << QStringLiteral("\\b__FILE__\\b")
                    << QStringLiteral("\\b__FUNCTION__\\b") << QStringLiteral("\\b__LINE__\\b")
                    << QStringLiteral("\\b__METHOD__\\b") << QStringLiteral("\\b__NAMESPACE__\\b")
                    << QStringLiteral("\\b__TRAIT__\\b");

    HighlightingRule rule;
    for (const QString& pattern : keywordPatterns)
    {
        rule.startPattern_ = QRegularExpression(pattern);
        rule.format_ = keywordFormat;
        highlightingRules_.append(rule);
    }

    QTextCharFormat functionFormat;
    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.startPattern_ = QRegularExpression(QStringLiteral(R"(\b[A-Za-z0-9_]+\s*(?=\())"));
    rule.format_ = functionFormat;
    highlightingRules_.append(rule);

    QTextCharFormat quotationFormat;
    quotationFormat.setForeground(Qt::darkGreen);
    rule.startPattern_ = QRegularExpression(QStringLiteral(R"(("([^"]|\\")*"|'([^']|\\')*'))"));
    rule.format_ = quotationFormat;
    highlightingRules_.append(rule);
}

void PhpHighlighter::commentBlock(const QString& text)
{
    singleLineComment(text, singleLineCommentRule_);
    multiLineComment(text, multiLineCommentRule_);
}
