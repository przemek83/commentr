#include "SQLHighlighter.h"

SQLHighlighter::SQLHighlighter(QObject* parent) : Highlighter(parent)
{
    singleLineCommentRule_.format.setForeground(Qt::red);
    singleLineCommentRule_.startPattern = QRegularExpression("--[^\n]*");

    multiLineCommentRule_.startPattern = QRegularExpression("/\\*");
    multiLineCommentRule_.endPattern = QRegularExpression("\\*/");
    multiLineCommentRule_.format.setForeground(Qt::red);
}

void SQLHighlighter::initRules()
{
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;

    keywordPatterns
        << "\\badd\\b" << "\\bexternal\\b" << "\\bprocedure\\b" << "\\ball\\b"
        << "\\bfetch\\b" << "\\bpublic\\b" << "\\balter\\b" << "\\bfile\\b"
        << "\\braiserror\\b" << "\\band\\b" << "\\bfillfactor\\b"
        << "\\bread\\b" << "\\bany\\b" << "\\bfor\\b" << "\\breadtext\\b"
        << "\\bas\\b" << "\\bforeign\\b" << "\\breconfigure\\b" << "\\basc\\b"
        << "\\bfreetext\\b" << "\\breferences\\b" << "\\bauthorization\\b"
        << "\\bfreetexttable\\b" << "\\breplication\\b" << "\\bbackup\\b"
        << "\\bfrom\\b" << "\\brestore\\b" << "\\bbegin\\b" << "\\bfull\\b"
        << "\\brestrict\\b" << "\\bbetween\\b" << "\\bfunction\\b"
        << "\\breturn\\b" << "\\bbreak\\b" << "\\bgoto\\b" << "\\brevert\\b"
        << "\\bbrowse\\b" << "\\bgrant\\b" << "\\brevoke\\b" << "\\bbulk\\b"
        << "\\bgroup\\b" << "\\bright\\b" << "\\bby\\b" << "\\bhaving\\b"
        << "\\brollback\\b" << "\\bcascade\\b" << "\\bholdlock\\b"
        << "\\browcount\\b" << "\\bcase\\b" << "\\bidentity\\b"
        << "\\browguidcol\\b" << "\\bcheck\\b" << "\\bidentity_insert\\b"
        << "\\brule\\b" << "\\bcheckpoint\\b" << "\\bidentitycol\\b"
        << "\\bsave\\b" << "\\bclose\\b" << "\\bif\\b" << "\\bschema\\b"
        << "\\bclustered\\b" << "\\bin\\b" << "\\bsecurityaudit\\b"
        << "\\bcoalesce\\b" << "\\bindex\\b" << "\\bselect\\b"
        << "\\bcollate\\b" << "\\binner\\b" << "\\bsemantickeyphrasetable\\b"
        << "\\bcolumn\\b" << "\\binsert\\b"
        << "\\bsemanticsimilaritydetailstable\\b" << "\\bcommit\\b"
        << "\\bintersect\\b" << "\\bsemanticsimilaritytable\\b"
        << "\\bcompute\\b" << "\\binto\\b" << "\\bsession_user\\b"
        << "\\bconstraint\\b" << "\\bis\\b" << "\\bset\\b" << "\\bcontains\\b"
        << "\\bjoin\\b" << "\\bsetuser\\b" << "\\bcontainstable\\b"
        << "\\bkey\\b" << "\\bshutdown\\b" << "\\bcontinue\\b" << "\\bkill\\b"
        << "\\bsome\\b" << "\\bconvert\\b" << "\\bleft\\b" << "\\bstatistics\\b"
        << "\\bcreate\\b" << "\\blike\\b" << "\\bsystem_user\\b"
        << "\\bcross\\b" << "\\blineno\\b" << "\\btable\\b" << "\\bcurrent\\b"
        << "\\bload\\b" << "\\btablesample\\b" << "\\bcurrent_date\\b"
        << "\\bmerge\\b" << "\\btextsize\\b" << "\\bcurrent_time\\b"
        << "\\bnational\\b" << "\\bthen\\b" << "\\bcurrent_timestamp\\b"
        << "\\bnocheck\\b" << "\\bto\\b" << "\\bcurrent_user\\b"
        << "\\bnonclustered\\b" << "\\btop\\b" << "\\bcursor\\b" << "\\bnot\\b"
        << "\\btran\\b" << "\\bdatabase\\b" << "\\bnull\\b"
        << "\\btransaction\\b" << "\\bdbcc\\b" << "\\bnullif\\b"
        << "\\btrigger\\b" << "\\bdeallocate\\b" << "\\bof\\b"
        << "\\btruncate\\b" << "\\bdeclare\\b" << "\\boff\\b"
        << "\\btry_convert\\b" << "\\bdefault\\b" << "\\boffsets\\b"
        << "\\btsequal\\b" << "\\bdelete\\b" << "\\bon\\b" << "\\bunion\\b"
        << "\\bdeny\\b" << "\\bopen\\b" << "\\bunique\\b" << "\\bdesc\\b"
        << "\\bopendatasource\\b" << "\\bunpivot\\b" << "\\bdisk\\b"
        << "\\bopenquery\\b" << "\\bupdate\\b" << "\\bdistinct\\b"
        << "\\bopenrowset\\b" << "\\bupdatetext\\b" << "\\bdistributed\\b"
        << "\\bopenxml\\b" << "\\buse\\b" << "\\bdouble\\b" << "\\boption\\b"
        << "\\buser\\b" << "\\bdrop\\b" << "\\bor\\b" << "\\bvalues\\b"
        << "\\bdump\\b" << "\\border\\b" << "\\bvarying\\b" << "\\belse\\b"
        << "\\bouter\\b" << "\\bview\\b" << "\\bend\\b" << "\\bover\\b"
        << "\\bwaitfor\\b" << "\\berrlvl\\b" << "\\bpercent\\b" << "\\bwhen\\b"
        << "\\bescape\\b" << "\\bpivot\\b" << "\\bwhere\\b" << "\\bexcept\\b"
        << "\\bplan\\b" << "\\bwhile\\b" << "\\bexec\\b" << "\\bprecision\\b"
        << "\\bwith\\b" << "\\bexecute\\b" << "\\bprimary\\b"
        << "\\bwithin group\\b" << "\\bexists\\b" << "\\bprint\\b"
        << "\\bwritetext\\b" << "\\bexit\\b" << "\\bproc\\b";

    HighlightingRule rule;
    foreach (const QString& pattern, keywordPatterns)
    {
        rule.startPattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules_.append(rule);
    }

    QTextCharFormat quotationFormat;
    quotationFormat.setForeground(Qt::darkGreen);
    rule.startPattern = QRegularExpression("(\"[^\"]*\"|\'[^\']*\')");
    rule.format = quotationFormat;
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
