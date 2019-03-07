#include "fmfileiconprovider.h"

#include <QFileInfo>

FMFileIconProvider::FMFileIconProvider()
{
    mimeDataBase = new QMimeDatabase;
}

//Qt can't get theme icon directly. use QIcon::fromTheme can, but we need setThemeName first.
QIcon FMFileIconProvider::icon(const QFileInfo &info) const {
    return QIcon::fromTheme((mimeDataBase->mimeTypeForFile(info)).iconName());
}
