#include "ObjectiveCHighlighter.h"

ObjectiveCHighlighter::ObjectiveCHighlighter(QObject * parent) :
    CFamilyHighlighter(parent)
{

}

ObjectiveCHighlighter::~ObjectiveCHighlighter()
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

    keywordPatterns << "\\bauto\\b" << "\\bBOOL\\b" << "\\bbreak\\b" <<
       "\\bClass\\b" << "\\bcase\\b" << "\\bbycopy\\b" <<
       "\\bchar\\b" << "\\bbyref\\b" << "\\bconst\\b" <<
       "\\bid\\b" << "\\bcontinue\\b" << "\\bIMP\\b" <<
       "\\bdefault\\b" << "\\bin\\b" << "\\bdo\\b" <<
       "\\binout\\b" << "\\bdouble\\b" << "\\bnil\\b" <<
       "\\belse\\b" << "\\bNO\\b" << "\\benum\\b" <<
       "\\bNULL\\b" << "\\bextern\\b" << "\\boneway\\b" <<
       "\\bfloat\\b" << "\\bout\\b" << "\\bfor\\b" <<
       "\\bProtocol\\b" << "\\bgoto\\b" << "\\bSEL\\b" <<
       "\\bif\\b" << "\\bself\\b" << "\\binline\\b" <<
       "\\bsuper\\b" << "\\bint\\b" << "\\bYES\\b" <<
       "\\blong\\b" << "(^|\\s)@interface\\b" << "\\bregister\\b" <<
       "(^|\\s)@end\\b" << "\\brestrict\\b" << "(^|\\s)@implementation\\b" <<
       "\\breturn\\b" << "(^|\\s)@protocol\\b" << "\\bshort\\b" <<
       "(^|\\s)@class\\b" << "\\bsigned\\b" << "(^|\\s)@public\\b" <<
       "\\bsizeof\\b" << "(^|\\s)@protected\\b" << "\\bstatic\\b" <<
       "(^|\\s)@private\\b" << "\\bstruct\\b" << "(^|\\s)@property\\b" <<
       "\\bswitch\\b" << "(^|\\s)@try\\b" << "\\btypedef\\b" <<
       "(^|\\s)@throw\\b" << "\\bunion\\b" << "(^|\\s)@catch()\\b" <<
       "\\bunsigned\\b" << "(^|\\s)@finally\\b" << "\\bvoid\\b" <<
       "(^|\\s)@synthesize\\b" << "\\bvolatile\\b" << "(^|\\s)@dynamic\\b" <<
       "\\bwhile\\b" << "(^|\\s)@selector\\b" << "\\b_Bool\\b" <<
       "\\batomic\\b" << "\\b_Complex\\b" << "\\bnonatomic\\b" <<
       "\\b_Imaginery\\b" << "\\bretain\\b" << "(^|\\s)@defs" <<
       "(^|\\s)@required" << "(^|\\s)@optional" <<
       "(^|\\s)@package" << "(^|\\s)@synchronized" <<
       "(^|\\s)@autoreleasepool" << "(^|\\s)@encode" <<
       "(^|\\s)@compatibility_alias" << "(^|\\s)@encode";

    //Preprocessor keywords.
    keywordPatterns << "(^|\\s)#if\\b" << "(^|\\s)#define\\b" <<
        "(^|\\s)#include\\b" << "(^|\\s)#ifdef\\b" << "(^|\\s)#undef\\b" <<
        "(^|\\s)#ifndef\\b" << "(^|\\s)#endif\\b" << "(^|\\s)#else\\b" <<
        "(^|\\s)#import\\b" << "(^|\\s)#elif\\b";;

    foreach (const QString &pattern, keywordPatterns)
    {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules_.append(rule);
    }

    QTextCharFormat classFormat;
    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegularExpression("\\bNS[A-Za-z]+\\b");
    rule.format = classFormat;
    highlightingRules_.append(rule);

    initQuotationRules();
}
