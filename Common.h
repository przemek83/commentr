#ifndef COMMON_H
#define COMMON_H

#include <QString>

class QWidget;
class QObject;

class Common
{
public:
    static QString loadFile(QString name);

    static QString saveFile(QString fileName, QString& data);

    static QString getStyleSheet();

    static int timerFireInterval();

    static const QString rootPath();

    enum Source
    {
        SOURCE_LOCAL = 0,
        SOURCE_FTP,
        SOURCE_NOT_SET
    };

    static QWidget* getMainWindow(QObject* startObject);

    static void centerWidget(QObject* hierarchyObject,
                                     QWidget* widgetToCenter);

    static int getMaxRecentFiles();

    static float normalizeFont(float fontSize);

private:
    Common();
    virtual ~Common();
    Common(const Common &);
    Common &operator=(const Common &);

    static const int timerFireInterval_;

    const static char* rootPath_;

    const static int maxRecentFiles_;
};

#endif // COMMON_H
