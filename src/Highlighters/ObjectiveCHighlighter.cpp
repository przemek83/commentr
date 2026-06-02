#include "ObjectiveCHighlighter.h"

ObjectiveCHighlighter::ObjectiveCHighlighter(const SpellChecker& spellChecker,
                                             QObject* parent)
    : CFamilyHighlighter(spellChecker, parent)
{
}

void ObjectiveCHighlighter::initRules()
{
    initFunctionsRules();

    HighlightingRule rule;

    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;

    keywordPatterns
        << QStringLiteral("\\bauto\\b") << QStringLiteral("\\bBOOL\\b")
        << QStringLiteral("\\bbreak\\b") << QStringLiteral("\\bClass\\b")
        << QStringLiteral("\\bcase\\b") << QStringLiteral("\\bbycopy\\b")
        << QStringLiteral("\\bchar\\b") << QStringLiteral("\\bbyref\\b")
        << QStringLiteral("\\bconst\\b") << QStringLiteral("\\bid\\b")
        << QStringLiteral("\\bcontinue\\b") << QStringLiteral("\\bIMP\\b")
        << QStringLiteral("\\bdefault\\b") << QStringLiteral("\\bin\\b")
        << QStringLiteral("\\bdo\\b") << QStringLiteral("\\binout\\b")
        << QStringLiteral("\\bdouble\\b") << QStringLiteral("\\bnil\\b")
        << QStringLiteral("\\belse\\b") << QStringLiteral("\\bNO\\b")
        << QStringLiteral("\\benum\\b") << QStringLiteral("\\bNULL\\b")
        << QStringLiteral("\\bextern\\b") << QStringLiteral("\\boneway\\b")
        << QStringLiteral("\\bfloat\\b") << QStringLiteral("\\bout\\b")
        << QStringLiteral("\\bfor\\b") << QStringLiteral("\\bProtocol\\b")
        << QStringLiteral("\\bgoto\\b") << QStringLiteral("\\bSEL\\b")
        << QStringLiteral("\\bif\\b") << QStringLiteral("\\bself\\b")
        << QStringLiteral("\\binline\\b") << QStringLiteral("\\bsuper\\b")
        << QStringLiteral("\\bint\\b") << QStringLiteral("\\bYES\\b")
        << QStringLiteral("\\blong\\b")
        << QStringLiteral("(^|\\s)@interface\\b")
        << QStringLiteral("\\bregister\\b") << QStringLiteral("(^|\\s)@end\\b")
        << QStringLiteral("\\brestrict\\b")
        << QStringLiteral("(^|\\s)@implementation\\b")
        << QStringLiteral("\\breturn\\b")
        << QStringLiteral("(^|\\s)@protocol\\b")
        << QStringLiteral("\\bshort\\b") << QStringLiteral("(^|\\s)@class\\b")
        << QStringLiteral("\\bsigned\\b") << QStringLiteral("(^|\\s)@public\\b")
        << QStringLiteral("\\bsizeof\\b")
        << QStringLiteral("(^|\\s)@protected\\b")
        << QStringLiteral("\\bstatic\\b")
        << QStringLiteral("(^|\\s)@private\\b")
        << QStringLiteral("\\bstruct\\b")
        << QStringLiteral("(^|\\s)@property\\b")
        << QStringLiteral("\\bswitch\\b") << QStringLiteral("(^|\\s)@try\\b")
        << QStringLiteral("\\btypedef\\b") << QStringLiteral("(^|\\s)@throw\\b")
        << QStringLiteral("\\bunion\\b") << QStringLiteral("(^|\\s)@catch()\\b")
        << QStringLiteral("\\bunsigned\\b")
        << QStringLiteral("(^|\\s)@finally\\b") << QStringLiteral("\\bvoid\\b")
        << QStringLiteral("(^|\\s)@synthesize\\b")
        << QStringLiteral("\\bvolatile\\b")
        << QStringLiteral("(^|\\s)@dynamic\\b") << QStringLiteral("\\bwhile\\b")
        << QStringLiteral("(^|\\s)@selector\\b")
        << QStringLiteral("\\b_Bool\\b") << QStringLiteral("\\batomic\\b")
        << QStringLiteral("\\b_Complex\\b") << QStringLiteral("\\bnonatomic\\b")
        << QStringLiteral("\\b_Imaginery\\b") << QStringLiteral("\\bretain\\b")
        << QStringLiteral("(^|\\s)@defs") << QStringLiteral("(^|\\s)@required")
        << QStringLiteral("(^|\\s)@optional")
        << QStringLiteral("(^|\\s)@package")
        << QStringLiteral("(^|\\s)@synchronized")
        << QStringLiteral("(^|\\s)@autoreleasepool")
        << QStringLiteral("(^|\\s)@encode")
        << QStringLiteral("(^|\\s)@compatibility_alias")
        << QStringLiteral("(^|\\s)@encode");

    // Preprocessor keywords.
    keywordPatterns << QStringLiteral("(^|\\s)#if\\b")
                    << QStringLiteral("(^|\\s)#define\\b")
                    << QStringLiteral("(^|\\s)#include\\b")
                    << QStringLiteral("(^|\\s)#ifdef\\b")
                    << QStringLiteral("(^|\\s)#undef\\b")
                    << QStringLiteral("(^|\\s)#ifndef\\b")
                    << QStringLiteral("(^|\\s)#endif\\b")
                    << QStringLiteral("(^|\\s)#else\\b")
                    << QStringLiteral("(^|\\s)#import\\b")
                    << QStringLiteral("(^|\\s)#elif\\b");

    for (const QString& pattern : keywordPatterns)
    {
        rule.startPattern_ = QRegularExpression(pattern);
        rule.format_ = keywordFormat;
        highlightingRules_.append(rule);
    }

    QTextCharFormat classFormat;
    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.startPattern_ =
        QRegularExpression(QStringLiteral("\\bNS[A-Za-z]+\\b"));
    rule.format_ = classFormat;
    highlightingRules_.append(rule);

    initQuotationRules();
}
