#include <QDebug>

#include "Highlighter.h"
#include "SpellChecker.h"

bool Highlighter::spellChecking_ = true;

Highlighter::Highlighter(QObject * parent):
    QSyntaxHighlighter(parent),
    initialized_(false)
{
    spellCheckFormat_.setForeground(Qt::red);
    spellCheckFormat_.setUnderlineColor(QColor(Qt::red));
    spellCheckFormat_.setUnderlineStyle(QTextCharFormat::WaveUnderline);
}

Highlighter::~Highlighter()
{

}

void Highlighter::highlightBlock(const QString& text)
{
    if( false == initialized_ )
    {
        initRules();
        initialized_ = true;
    }

    foreach (const HighlightingRule &rule, highlightingRules_)
    {
        const QRegularExpression& expression = rule.pattern;
        for (const auto& match : expression.globalMatch(text))
        {
            int length = match.capturedLength();
            int index = match.capturedStart();
            setFormat(index, length, rule.format);
        }
    }

    //Add comments highlight.
    commentBlock(text);
}

void Highlighter::checkSpellingInBlock(int minIndex, const QString& line)
{
    if( false == spellChecking_ )
    {
        return;
    }

    QString str = line.simplified();
    QStringList wordsList = str.split(QRegularExpression("([^\\w,^\\\\]|(?=\\\\))+"),
                                       Qt::SkipEmptyParts);
    foreach(QString word, wordsList)
    {
        if ( word.length() > 1 && false == word.startsWith('\\') &&
             false == SpellChecker::getInstance().checkWord(word) )
        {
            int l = -1, number = 0;
            number = line.count(QRegularExpression("\\b" + word + "\\b"));
            for ( int j=0; j < number; ++j )
            {
                l = line.indexOf(QRegularExpression("\\b" + word + "\\b"), minIndex + l + 1);
                if ( l >= 0 )
                {
                    setFormat(l, word.length(), spellCheckFormat_);
                }
            }
        }
    }
}

void Highlighter::singleLineComment(const QString& text, const HighlightingRule& rule)
{
    const QRegularExpression& expression = rule.pattern;
    QRegularExpressionMatch match = expression.match(text);
    if (match.hasMatch())
    {
        int length = match.capturedLength();
        int index = match.capturedStart();
        setFormat(index, length, rule.format);
        checkSpellingInBlock(index, text);
    }
}

void Highlighter::setSpellChecking(bool check)
{
    spellChecking_ = check;

}
