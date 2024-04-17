#include "BasicHighlighter.h"

BasicHighlighter::BasicHighlighter(QObject * parent) :
    Highlighter(parent)
{
    singleLineCommentRule_.format.setForeground(Qt::red);
    singleLineCommentRule_.pattern = QRegExp("'[^\n]*");
}

BasicHighlighter::~BasicHighlighter()
{

}

void BasicHighlighter::initRules()
{
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;

    keywordPatterns << "\\bAddHandler\\b" << "\\bAddressOf\\b" << "\\bAlias\\b" <<
        "\\bAnd\\b" << "\\bAndAlso\\b" << "\\bAs\\b" << "\\bBoolean\\b" <<
        "\\bByRef\\b" << "\\bByte\\b" << "\\bByVal\\b" << "\\bCall\\b" <<
        "\\bCase\\b" << "\\bCatch\\b" << "\\bCBool\\b" << "\\bCByte\\b" <<
        "\\bCChar\\b" << "\\bCDate\\b" << "\\bCDec\\b" << "\\bCDbl\\b" <<
        "\\bChar\\b" << "\\bCInt\\b" << "\\bClass\\b" << "\\bCLng\\b" <<
        "\\bCObj\\b" << "\\bConst\\b" << "\\bContinue\\b" << "\\bCSByte\\b" <<
        "\\bCShort\\b" << "\\bCSng\\b" << "\\bCStr\\b" << "\\bCType\\b" <<
        "\\bCUInt\\b" << "\\bCULng\\b" << "\\bCUShort\\b" << "\\bDate\\b" <<
        "\\bDecimal\\b" << "\\bDeclare\\b" << "\\bDefault\\b" << "\\bDelegate\\b" <<
        "\\bDim\\b" << "\\bDirectCast\\b" << "\\bDo\\b" << "\\bDouble\\b" <<
        "\\bEach\\b" << "\\bElse\\b" << "\\bElseIf\\b" << "\\bEnd\\b" <<
        "\\bEndIf\\b" << "\\bEnum\\b" << "\\bErase\\b" << "\\bError\\b" <<
        "\\bEvent\\b" << "\\bExit\\b" << "\\bFalse\\b" << "\\bFinally\\b" <<
        "\\bFor\\b" << "\\bFriend\\b" << "\\bFunction\\b" << "\\bGet\\b" <<
        "\\bGetType\\b" << "\\bGetXMLNamespace\\b" << "\\bGlobal\\b" <<
        "\\bGoSub\\b" << "\\bGoTo\\b" << "\\bHandles\\b" << "\\bIf\\b" <<
        "\\bImplements\\b" << "\\bImports\\b" << "\\bImports\\b" << "\\bIn\\b" <<
        "\\bInherits\\b" << "\\bInteger\\b" << "\\bInterface\\b" << "\\bIs\\b" <<
        "\\bIsNot\\b" << "\\bLet\\b" << "\\bLib\\b" << "\\bLike\\b" << "\\bLong\\b" <<
        "\\bLoop\\b" << "\\bMe\\b" << "\\bMod\\b" << "\\bModule\\b" <<
        "\\bMustInherit\\b" << "\\bMustOverride\\b" << "\\bMyBase\\b" <<
        "\\bMyClass\\b" << "\\bNamespace\\b" << "\\bNarrowing\\b" << "\\bNew\\b" <<
        "\\bNext\\b" << "\\bNot\\b" << "\\bNothing\\b" << "\\bNotInheritable\\b" <<
        "\\bNotOverridable\\b" << "\\bObject\\b" << "\\bOf\\b" << "\\bOn\\b" <<
        "\\bOperator\\b" << "\\bOption\\b" << "\\bOptional\\b" << "\\bOr\\b" <<
        "\\bOrElse\\b" << "\\bOverloads\\b" << "\\bOverridable\\b" <<
        "\\bOverrides\\b" << "\\bParamArray\\b" << "\\bPartial\\b" <<
        "\\bPrivate\\b" << "\\bProperty\\b" << "\\bProtected\\b" << "\\bPublic\\b" <<
        "\\bRaiseEvent\\b" << "\\bReadOnly\\b" << "\\bReDim\\b" << "\\bREM\\b" <<
        "\\bRemoveHandler\\b" << "\\bResume\\b" << "\\bReturn\\b" << "\\bSByte\\b" <<
        "\\bSelect\\b" << "\\bSet\\b" << "\\bShadows\\b" << "\\bShared\\b" <<
        "\\bShort\\b" << "\\bSingle\\b" << "\\bStatic\\b" << "\\bStep\\b" <<
        "\\bStop\\b" << "\\bString\\b" << "\\bStructure\\b" << "\\bSub\\b" <<
        "\\bSyncLock\\b" << "\\bThen\\b" << "\\bThrow\\b" << "\\bTo\\b" <<
        "\\bTrue\\b" << "\\bTry\\b" << "\\bTryCast\\b" << "\\bTypeOf\\b" <<
        "\\bVariant\\b" << "\\bWend\\b" << "\\bUInteger\\b" << "\\bULong\\b" <<
        "\\bUShort\\b" << "\\bUsing\\b" << "\\bWhen\\b" << "\\bWhile\\b" <<
        "\\bWidening\\b" << "\\bWith\\b" << "\\bWithEvents\\b" << "\\bWriteOnly\\b" <<
        "\\bXor\\b" << "(^|\\s)#Const\\b" << "(^|\\s)#Else\\b" << "(^|\\s)#ElseIf\\b" <<
        "(^|\\s)#End\\b" << "(^|\\s)#If\\b";

    HighlightingRule rule;
    foreach (const QString &pattern, keywordPatterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules_.append(rule);
    }

    QTextCharFormat quotationFormat;
    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("\".*\"");
    rule.format = quotationFormat;
    highlightingRules_.append(rule);

    QTextCharFormat functionFormat;
    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+\\s*(?=\\()");
    rule.format = functionFormat;
    highlightingRules_.append(rule);
}

void BasicHighlighter::commentBlock(const QString& text)
{
    //Single line comment.
    const QRegExp& expression = singleLineCommentRule_.pattern;
    int index = expression.indexIn(text);
    if (index >= 0)
    {
        int length = expression.matchedLength();
        setFormat(index, length, singleLineCommentRule_.format);
        checkSpellingInBlock(index, text);
    }
}

