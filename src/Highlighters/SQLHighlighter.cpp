#include "SQLHighlighter.h"

SQLHighlighter::SQLHighlighter(const SpellChecker& spellChecker,
                               QObject* parent)
    : Highlighter(spellChecker, parent)
{
    singleLineCommentRule_.format_.setForeground(Qt::red);
    singleLineCommentRule_.startPattern_ =
        QRegularExpression(QStringLiteral("--[^\n]*"));

    multiLineCommentRule_.startPattern_ =
        QRegularExpression(QStringLiteral("/\\*"));
    multiLineCommentRule_.endPattern_ =
        QRegularExpression(QStringLiteral("\\*/"));
    multiLineCommentRule_.format_.setForeground(Qt::red);
}

void SQLHighlighter::initRules()
{
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;

    keywordPatterns
        << QStringLiteral("\\badd\\b") << QStringLiteral("\\bexternal\\b")
        << QStringLiteral("\\bprocedure\\b") << QStringLiteral("\\ball\\b")
        << QStringLiteral("\\bfetch\\b") << QStringLiteral("\\bpublic\\b")
        << QStringLiteral("\\balter\\b") << QStringLiteral("\\bfile\\b")
        << QStringLiteral("\\braiserror\\b") << QStringLiteral("\\band\\b")
        << QStringLiteral("\\bfillfactor\\b") << QStringLiteral("\\bread\\b")
        << QStringLiteral("\\bany\\b") << QStringLiteral("\\bfor\\b")
        << QStringLiteral("\\breadtext\\b") << QStringLiteral("\\bas\\b")
        << QStringLiteral("\\bforeign\\b")
        << QStringLiteral("\\breconfigure\\b") << QStringLiteral("\\basc\\b")
        << QStringLiteral("\\bfreetext\\b")
        << QStringLiteral("\\breferences\\b")
        << QStringLiteral("\\bauthorization\\b")
        << QStringLiteral("\\bfreetexttable\\b")
        << QStringLiteral("\\breplication\\b") << QStringLiteral("\\bbackup\\b")
        << QStringLiteral("\\bfrom\\b") << QStringLiteral("\\brestore\\b")
        << QStringLiteral("\\bbegin\\b") << QStringLiteral("\\bfull\\b")
        << QStringLiteral("\\brestrict\\b") << QStringLiteral("\\bbetween\\b")
        << QStringLiteral("\\bfunction\\b") << QStringLiteral("\\breturn\\b")
        << QStringLiteral("\\bbreak\\b") << QStringLiteral("\\bgoto\\b")
        << QStringLiteral("\\brevert\\b") << QStringLiteral("\\bbrowse\\b")
        << QStringLiteral("\\bgrant\\b") << QStringLiteral("\\brevoke\\b")
        << QStringLiteral("\\bbulk\\b") << QStringLiteral("\\bgroup\\b")
        << QStringLiteral("\\bright\\b") << QStringLiteral("\\bby\\b")
        << QStringLiteral("\\bhaving\\b") << QStringLiteral("\\brollback\\b")
        << QStringLiteral("\\bcascade\\b") << QStringLiteral("\\bholdlock\\b")
        << QStringLiteral("\\browcount\\b") << QStringLiteral("\\bcase\\b")
        << QStringLiteral("\\bidentity\\b")
        << QStringLiteral("\\browguidcol\\b") << QStringLiteral("\\bcheck\\b")
        << QStringLiteral("\\bidentity_insert\\b")
        << QStringLiteral("\\brule\\b") << QStringLiteral("\\bcheckpoint\\b")
        << QStringLiteral("\\bidentitycol\\b") << QStringLiteral("\\bsave\\b")
        << QStringLiteral("\\bclose\\b") << QStringLiteral("\\bif\\b")
        << QStringLiteral("\\bschema\\b") << QStringLiteral("\\bclustered\\b")
        << QStringLiteral("\\bin\\b") << QStringLiteral("\\bsecurityaudit\\b")
        << QStringLiteral("\\bcoalesce\\b") << QStringLiteral("\\bindex\\b")
        << QStringLiteral("\\bselect\\b") << QStringLiteral("\\bcollate\\b")
        << QStringLiteral("\\binner\\b")
        << QStringLiteral("\\bsemantickeyphrasetable\\b")
        << QStringLiteral("\\bcolumn\\b") << QStringLiteral("\\binsert\\b")
        << QStringLiteral("\\bsemanticsimilaritydetailstable\\b")
        << QStringLiteral("\\bcommit\\b") << QStringLiteral("\\bintersect\\b")
        << QStringLiteral("\\bsemanticsimilaritytable\\b")
        << QStringLiteral("\\bcompute\\b") << QStringLiteral("\\binto\\b")
        << QStringLiteral("\\bsession_user\\b")
        << QStringLiteral("\\bconstraint\\b") << QStringLiteral("\\bis\\b")
        << QStringLiteral("\\bset\\b") << QStringLiteral("\\bcontains\\b")
        << QStringLiteral("\\bjoin\\b") << QStringLiteral("\\bsetuser\\b")
        << QStringLiteral("\\bcontainstable\\b") << QStringLiteral("\\bkey\\b")
        << QStringLiteral("\\bshutdown\\b") << QStringLiteral("\\bcontinue\\b")
        << QStringLiteral("\\bkill\\b") << QStringLiteral("\\bsome\\b")
        << QStringLiteral("\\bconvert\\b") << QStringLiteral("\\bleft\\b")
        << QStringLiteral("\\bstatistics\\b") << QStringLiteral("\\bcreate\\b")
        << QStringLiteral("\\blike\\b") << QStringLiteral("\\bsystem_user\\b")
        << QStringLiteral("\\bcross\\b") << QStringLiteral("\\blineno\\b")
        << QStringLiteral("\\btable\\b") << QStringLiteral("\\bcurrent\\b")
        << QStringLiteral("\\bload\\b") << QStringLiteral("\\btablesample\\b")
        << QStringLiteral("\\bcurrent_date\\b") << QStringLiteral("\\bmerge\\b")
        << QStringLiteral("\\btextsize\\b")
        << QStringLiteral("\\bcurrent_time\\b")
        << QStringLiteral("\\bnational\\b") << QStringLiteral("\\bthen\\b")
        << QStringLiteral("\\bcurrent_timestamp\\b")
        << QStringLiteral("\\bnocheck\\b") << QStringLiteral("\\bto\\b")
        << QStringLiteral("\\bcurrent_user\\b")
        << QStringLiteral("\\bnonclustered\\b") << QStringLiteral("\\btop\\b")
        << QStringLiteral("\\bcursor\\b") << QStringLiteral("\\bnot\\b")
        << QStringLiteral("\\btran\\b") << QStringLiteral("\\bdatabase\\b")
        << QStringLiteral("\\bnull\\b") << QStringLiteral("\\btransaction\\b")
        << QStringLiteral("\\bdbcc\\b") << QStringLiteral("\\bnullif\\b")
        << QStringLiteral("\\btrigger\\b") << QStringLiteral("\\bdeallocate\\b")
        << QStringLiteral("\\bof\\b") << QStringLiteral("\\btruncate\\b")
        << QStringLiteral("\\bdeclare\\b") << QStringLiteral("\\boff\\b")
        << QStringLiteral("\\btry_convert\\b")
        << QStringLiteral("\\bdefault\\b") << QStringLiteral("\\boffsets\\b")
        << QStringLiteral("\\btsequal\\b") << QStringLiteral("\\bdelete\\b")
        << QStringLiteral("\\bon\\b") << QStringLiteral("\\bunion\\b")
        << QStringLiteral("\\bdeny\\b") << QStringLiteral("\\bopen\\b")
        << QStringLiteral("\\bunique\\b") << QStringLiteral("\\bdesc\\b")
        << QStringLiteral("\\bopendatasource\\b")
        << QStringLiteral("\\bunpivot\\b") << QStringLiteral("\\bdisk\\b")
        << QStringLiteral("\\bopenquery\\b") << QStringLiteral("\\bupdate\\b")
        << QStringLiteral("\\bdistinct\\b")
        << QStringLiteral("\\bopenrowset\\b")
        << QStringLiteral("\\bupdatetext\\b")
        << QStringLiteral("\\bdistributed\\b")
        << QStringLiteral("\\bopenxml\\b") << QStringLiteral("\\buse\\b")
        << QStringLiteral("\\bdouble\\b") << QStringLiteral("\\boption\\b")
        << QStringLiteral("\\buser\\b") << QStringLiteral("\\bdrop\\b")
        << QStringLiteral("\\bor\\b") << QStringLiteral("\\bvalues\\b")
        << QStringLiteral("\\bdump\\b") << QStringLiteral("\\border\\b")
        << QStringLiteral("\\bvarying\\b") << QStringLiteral("\\belse\\b")
        << QStringLiteral("\\bouter\\b") << QStringLiteral("\\bview\\b")
        << QStringLiteral("\\bend\\b") << QStringLiteral("\\bover\\b")
        << QStringLiteral("\\bwaitfor\\b") << QStringLiteral("\\berrlvl\\b")
        << QStringLiteral("\\bpercent\\b") << QStringLiteral("\\bwhen\\b")
        << QStringLiteral("\\bescape\\b") << QStringLiteral("\\bpivot\\b")
        << QStringLiteral("\\bwhere\\b") << QStringLiteral("\\bexcept\\b")
        << QStringLiteral("\\bplan\\b") << QStringLiteral("\\bwhile\\b")
        << QStringLiteral("\\bexec\\b") << QStringLiteral("\\bprecision\\b")
        << QStringLiteral("\\bwith\\b") << QStringLiteral("\\bexecute\\b")
        << QStringLiteral("\\bprimary\\b")
        << QStringLiteral("\\bwithin group\\b")
        << QStringLiteral("\\bexists\\b") << QStringLiteral("\\bprint\\b")
        << QStringLiteral("\\bwritetext\\b") << QStringLiteral("\\bexit\\b")
        << QStringLiteral("\\bproc\\b");

    HighlightingRule rule;
    for (const QString& pattern : keywordPatterns)
    {
        rule.startPattern_ = QRegularExpression(pattern);
        rule.format_ = keywordFormat;
        highlightingRules_.append(rule);
    }

    QTextCharFormat quotationFormat;
    quotationFormat.setForeground(Qt::darkGreen);
    rule.startPattern_ =
        QRegularExpression(QStringLiteral(R"(("[^"]*"|'[^']*'))"));
    rule.format_ = quotationFormat;
    highlightingRules_.append(rule);
}

void SQLHighlighter::highlightBlock(const QString& text)
{
    QString lower(text.toLower());
    Highlighter::highlightBlock(lower);
}

void SQLHighlighter::commentBlock(const QString& text)
{
    singleLineComment(text, singleLineCommentRule_);
    multiLineComment(text, multiLineCommentRule_);
}
