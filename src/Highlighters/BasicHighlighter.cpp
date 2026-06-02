#include "BasicHighlighter.h"

BasicHighlighter::BasicHighlighter(const SpellChecker& spellChecker,
                                   QObject* parent)
    : Highlighter(spellChecker, parent)
{
    singleLineCommentRule_.format_.setForeground(Qt::red);
    singleLineCommentRule_.startPattern_ =
        QRegularExpression(QStringLiteral("'[^\n]*"));
}

void BasicHighlighter::initRules()
{
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    // clang-format off
    const QStringList keywordPatterns{
        QStringLiteral("\\bAddHandler\\b"),      QStringLiteral("\\bAddressOf\\b"),      QStringLiteral("\\bAlias\\b"),
        QStringLiteral("\\bAnd\\b"),             QStringLiteral("\\bAndAlso\\b"),        QStringLiteral("\\bAs\\b"),
        QStringLiteral("\\bBoolean\\b"),         QStringLiteral("\\bByRef\\b"),          QStringLiteral("\\bByte\\b"),
        QStringLiteral("\\bByVal\\b"),           QStringLiteral("\\bCall\\b"),           QStringLiteral("\\bCase\\b"),
        QStringLiteral("\\bCatch\\b"),           QStringLiteral("\\bCBool\\b"),          QStringLiteral("\\bCByte\\b"),
        QStringLiteral("\\bCChar\\b"),           QStringLiteral("\\bCDate\\b"),          QStringLiteral("\\bCDec\\b"),
        QStringLiteral("\\bCDbl\\b"),            QStringLiteral("\\bChar\\b"),           QStringLiteral("\\bCInt\\b"),
        QStringLiteral("\\bClass\\b"),           QStringLiteral("\\bCLng\\b"),           QStringLiteral("\\bCObj\\b"),
        QStringLiteral("\\bConst\\b"),           QStringLiteral("\\bContinue\\b"),       QStringLiteral("\\bCSByte\\b"),
        QStringLiteral("\\bCShort\\b"),          QStringLiteral("\\bCSng\\b"),           QStringLiteral("\\bCStr\\b"),
        QStringLiteral("\\bCType\\b"),           QStringLiteral("\\bCUInt\\b"),          QStringLiteral("\\bCULng\\b"),
        QStringLiteral("\\bCUShort\\b"),         QStringLiteral("\\bDate\\b"),           QStringLiteral("\\bDecimal\\b"),
        QStringLiteral("\\bDeclare\\b"),         QStringLiteral("\\bDefault\\b"),        QStringLiteral("\\bDelegate\\b"),
        QStringLiteral("\\bDim\\b"),             QStringLiteral("\\bDirectCast\\b"),     QStringLiteral("\\bDo\\b"),
        QStringLiteral("\\bDouble\\b"),          QStringLiteral("\\bEach\\b"),           QStringLiteral("\\bElse\\b"),
        QStringLiteral("\\bElseIf\\b"),          QStringLiteral("\\bEnd\\b"),            QStringLiteral("\\bEndIf\\b"),
        QStringLiteral("\\bEnum\\b"),            QStringLiteral("\\bErase\\b"),          QStringLiteral("\\bError\\b"),
        QStringLiteral("\\bEvent\\b"),           QStringLiteral("\\bExit\\b"),           QStringLiteral("\\bFalse\\b"),
        QStringLiteral("\\bFinally\\b"),         QStringLiteral("\\bFor\\b"),            QStringLiteral("\\bFriend\\b"),
        QStringLiteral("\\bFunction\\b"),        QStringLiteral("\\bGet\\b"),            QStringLiteral("\\bGetType\\b"),
        QStringLiteral("\\bGetXMLNamespace\\b"), QStringLiteral("\\bGlobal\\b"),         QStringLiteral("\\bGoSub\\b"),
        QStringLiteral("\\bGoTo\\b"),            QStringLiteral("\\bHandles\\b"),        QStringLiteral("\\bIf\\b"),
        QStringLiteral("\\bImplements\\b"),      QStringLiteral("\\bImports\\b"),        QStringLiteral("\\bIn\\b"),
        QStringLiteral("\\bInherits\\b"),        QStringLiteral("\\bInteger\\b"),        QStringLiteral("\\bInterface\\b"),
        QStringLiteral("\\bIs\\b"),              QStringLiteral("\\bIsNot\\b"),          QStringLiteral("\\bLet\\b"),
        QStringLiteral("\\bLib\\b"),             QStringLiteral("\\bLike\\b"),           QStringLiteral("\\bLong\\b"),
        QStringLiteral("\\bLoop\\b"),            QStringLiteral("\\bMe\\b"),             QStringLiteral("\\bMod\\b"),
        QStringLiteral("\\bModule\\b"),          QStringLiteral("\\bMustInherit\\b"),    QStringLiteral("\\bMustOverride\\b"),
        QStringLiteral("\\bMyBase\\b"),          QStringLiteral("\\bMyClass\\b"),        QStringLiteral("\\bNamespace\\b"),
        QStringLiteral("\\bNarrowing\\b"),       QStringLiteral("\\bNew\\b"),            QStringLiteral("\\bNext\\b"),
        QStringLiteral("\\bNot\\b"),             QStringLiteral("\\bNothing\\b"),        QStringLiteral("\\bNotInheritable\\b"),
        QStringLiteral("\\bNotOverridable\\b"),  QStringLiteral("\\bObject\\b"),         QStringLiteral("\\bOf\\b"),
        QStringLiteral("\\bOn\\b"),              QStringLiteral("\\bOperator\\b"),       QStringLiteral("\\bOption\\b"),
        QStringLiteral("\\bOptional\\b"),        QStringLiteral("\\bOr\\b"),             QStringLiteral("\\bOrElse\\b"),
        QStringLiteral("\\bOverloads\\b"),       QStringLiteral("\\bOverridable\\b"),    QStringLiteral("\\bOverrides\\b"),
        QStringLiteral("\\bParamArray\\b"),      QStringLiteral("\\bPartial\\b"),        QStringLiteral("\\bPrivate\\b"),
        QStringLiteral("\\bProperty\\b"),        QStringLiteral("\\bProtected\\b"),      QStringLiteral("\\bPublic\\b"),
        QStringLiteral("\\bRaiseEvent\\b"),      QStringLiteral("\\bReadOnly\\b"),       QStringLiteral("\\bReDim\\b"),
        QStringLiteral("\\bREM\\b"),             QStringLiteral("\\bRemoveHandler\\b"),  QStringLiteral("\\bResume\\b"),
        QStringLiteral("\\bReturn\\b"),          QStringLiteral("\\bSByte\\b"),          QStringLiteral("\\bSelect\\b"),
        QStringLiteral("\\bSet\\b"),             QStringLiteral("\\bShadows\\b"),        QStringLiteral("\\bShared\\b"),
        QStringLiteral("\\bShort\\b"),           QStringLiteral("\\bSingle\\b"),         QStringLiteral("\\bStatic\\b"),
        QStringLiteral("\\bStep\\b"),            QStringLiteral("\\bStop\\b"),           QStringLiteral("\\bString\\b"),
        QStringLiteral("\\bStructure\\b"),       QStringLiteral("\\bSub\\b"),            QStringLiteral("\\bSyncLock\\b"),
        QStringLiteral("\\bThen\\b"),            QStringLiteral("\\bThrow\\b"),          QStringLiteral("\\bTo\\b"),
        QStringLiteral("\\bTrue\\b"),            QStringLiteral("\\bTry\\b"),            QStringLiteral("\\bTryCast\\b"),
        QStringLiteral("\\bTypeOf\\b"),          QStringLiteral("\\bVariant\\b"),        QStringLiteral("\\bWend\\b"),
        QStringLiteral("\\bUInteger\\b"),        QStringLiteral("\\bULong\\b"),          QStringLiteral("\\bUShort\\b"),
        QStringLiteral("\\bUsing\\b"),           QStringLiteral("\\bWhen\\b"),           QStringLiteral("\\bWhile\\b"),
        QStringLiteral("\\bWidening\\b"),        QStringLiteral("\\bWith\\b"),           QStringLiteral("\\bWithEvents\\b"),
        QStringLiteral("\\bWriteOnly\\b"),       QStringLiteral("\\bXor\\b"),            QStringLiteral("(^|\\s)#Const\\b"),
        QStringLiteral("(^|\\s)#Else\\b"),       QStringLiteral("(^|\\s)#ElseIf\\b"),    QStringLiteral("(^|\\s)#End\\b"),
        QStringLiteral("(^|\\s)#If\\b")
    };
    // clang-format on

    HighlightingRule rule;
    for (const QString& pattern : keywordPatterns)
    {
        rule.startPattern_ = QRegularExpression(pattern);
        rule.format_ = keywordFormat;
        highlightingRules_.append(rule);
    }

    QTextCharFormat quotationFormat;
    quotationFormat.setForeground(Qt::darkGreen);
    rule.startPattern_ = QRegularExpression(QStringLiteral("\".*\""));
    rule.format_ = quotationFormat;
    highlightingRules_.append(rule);

    QTextCharFormat functionFormat;
    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.startPattern_ =
        QRegularExpression(QStringLiteral(R"(\b[A-Za-z0-9_]+\s*(?=\())"));
    rule.format_ = functionFormat;
    highlightingRules_.append(rule);
}

void BasicHighlighter::commentBlock(const QString& text)
{
    singleLineComment(text, singleLineCommentRule_);
}
