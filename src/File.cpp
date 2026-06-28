#include "File.h"

#include <QFileInfo>

File::File(Common::Source source, QString filePath, QString content)
    : source_(source),
      filePath_(std::move(filePath)),
      content_(std::move(content))
{
}

Common::Source File::source() const { return source_; }

void File::setSource(Common::Source source) { source_ = source; }

QString File::getFilePath() const { return filePath_; }
void File::setFilePath(const QString& filePath) { filePath_ = filePath; }

QString File::path() const { return QFileInfo(filePath_).path(); }

QString File::getFileName() const { return QFileInfo(filePath_).fileName(); }

QString File::suffix() const { return QFileInfo(filePath_).suffix(); }

const QString& File::content() const { return content_; }

void File::clearContent() { content_ = QString(); }
