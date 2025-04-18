#include "PhpHighlighter.h"

PhpHighlighter::PhpHighlighter(QObject* parent) : Highlighter(parent)
{
    singleLineCommentRule_.format.setForeground(Qt::red);
    singleLineCommentRule_.startPattern =
        QRegularExpression("(//[^\n]*|#[^\n]*)");

    multiLineCommentRule_.startPattern = QRegularExpression("/\\*");
    multiLineCommentRule_.endPattern = QRegularExpression("\\*/");
    multiLineCommentRule_.format.setForeground(Qt::red);
}

void PhpHighlighter::initRules()
{
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;

    keywordPatterns << "\\__halt_compiler\\b" << "\\babstract\\b" << "\\band\\b"
                    << "\\barray\\b" << "\\bas\\b" << "\\bbreak\\b"
                    << "\\bcallable\\b" << "\\bcase\\b" << "\\bcatch\\b"
                    << "\\bclass\\b" << "\\bclone\\b" << "\\bconst\\b"
                    << "\\bcontinue\\b" << "\\bdeclare\\b" << "\\bdefault\\b"
                    << "\\bdie\\b" << "\\bdo\\b" << "\\becho\\b" << "\\belse\\b"
                    << "\\belseif\\b" << "\\bempty\\b" << "\\benddeclare\\b"
                    << "\\bendfor\\b" << "\\bendforeach\\b" << "\\bendif\\b"
                    << "\\bendswitch\\b" << "\\bendwhile\\b" << "\\beval\\b"
                    << "\\bexit\\b" << "\\bextends\\b" << "\\bfinal\\b"
                    << "\\bfor\\b" << "\\bforeach\\b" << "\\bfunction\\b"
                    << "\\bglobal\\b" << "\\bgoto\\b" << "\\bif\\b"
                    << "\\bimplements\\b" << "\\binclude\\b"
                    << "\\binclude_once\\b" << "\\binstanceof\\b"
                    << "\\binsteadof\\b" << "\\binterface\\b" << "\\bisset\\b"
                    << "\\blist\\b" << "\\bnamespace\\b" << "\\bnew\\b"
                    << "\\bor\\b" << "\\bprint\\b" << "\\bprivate\\b"
                    << "\\bprotected\\b" << "\\bpublic\\b" << "\\brequire\\b"
                    << "\\brequire_once\\b" << "\\breturn\\b" << "\\bstatic\\b"
                    << "\\bswitch\\b" << "\\bthrow\\b" << "\\btrait\\b"
                    << "\\btry\\b" << "\\bunset\\b" << "\\buse\\b"
                    << "\\bvar\\b" << "\\bwhile\\b" << "\\bxor\\b"
                    << "\\byield\\b";

    keywordPatterns << "\\b__CLASS__\\b" << "\\b__DIR__\\b" << "\\b__FILE__\\b"
                    << "\\b__FUNCTION__\\b" << "\\b__LINE__\\b"
                    << "\\b__METHOD__\\b" << "\\b__NAMESPACE__\\b"
                    << "\\b__TRAIT__\\b";

    HighlightingRule rule;
    foreach (const QString& pattern, keywordPatterns)
    {
        rule.startPattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules_.append(rule);
    }

    QTextCharFormat functionFormat;
    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.startPattern = QRegularExpression("\\b[A-Za-z0-9_]+\\s*(?=\\()");
    rule.format = functionFormat;
    highlightingRules_.append(rule);

    QTextCharFormat quotationFormat;
    quotationFormat.setForeground(Qt::darkGreen);
    rule.startPattern =
        QRegularExpression("(\"([^\"]|\\\\\")*\"|\'([^\']|\\\\\')*\')");
    rule.format = quotationFormat;
    highlightingRules_.append(rule);
}

void PhpHighlighter::commentBlock(const QString& text)
{
    singleLineComment(text, singleLineCommentRule_);
    multiLineComment(text, multiLineCommentRule_);
}
