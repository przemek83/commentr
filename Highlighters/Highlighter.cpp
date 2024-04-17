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
        const QRegExp& expression = rule.pattern;
        int index = expression.indexIn(text);
        while (index >= 0)
        {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
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
    QStringList wordsList = str.split(QRegExp("([^\\w,^\\\\]|(?=\\\\))+"),
                                      QString::SkipEmptyParts);
    foreach(QString word, wordsList)
    {
        if ( word.length() > 1 && false == word.startsWith('\\') &&
             false == SpellChecker::getInstance().checkWord(word) )
        {
            int l = -1, number = 0;
            number = line.count(QRegExp("\\b" + word + "\\b"));
            for ( int j=0; j < number; ++j )
            {
                l = line.indexOf(QRegExp("\\b" + word + "\\b"), minIndex + l + 1);
                if ( l >= 0 )
                {
                    setFormat(l, word.length(), spellCheckFormat_);
                }
            }
        }
    }
}

void Highlighter::setSpellChecking(bool check)
{
    spellChecking_ = check;

}
