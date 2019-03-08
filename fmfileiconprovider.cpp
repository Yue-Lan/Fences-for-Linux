#include "fmfileiconprovider.h"

#include <QFileInfo>
#include <QPixmap>
#include <QFile>

#include <QUrl>
#include <QDebug>

FMFileIconProvider::FMFileIconProvider()
{
    mimeDataBase = new QMimeDatabase;
}

void FMFileIconProvider::setIconSize(int size) {
    if (size > 0)
        mIconSize = size;
}

//Qt can't get theme icon directly. use QIcon::fromTheme can, but we need setThemeName first.
QIcon FMFileIconProvider::icon(const QFileInfo &info) const {
    QPixmap tmpPixmap = QPixmap(info.filePath());
    if (!tmpPixmap.isNull()){
        tmpPixmap = tmpPixmap.scaled(mIconSize, mIconSize);
        return QIcon(tmpPixmap);
    }
    QIcon tmpIcon;
    tmpIcon.fromTheme(mimeDataBase->mimeTypeForFile(info).iconName());
    if (mimeDataBase->mimeTypeForUrl(QUrl(info.filePath())).aliases().isEmpty()) {
        /*
        qDebug()<<mimeDataBase->mimeTypeForUrl(QUrl(info.filePath())).name();
        qDebug()<<mimeDataBase->mimeTypeForUrl(QUrl(info.filePath())).iconName();
        qDebug()<<mimeDataBase->mimeTypeForUrl(QUrl(info.filePath())).aliases();
        */
        return QIcon::fromTheme(mimeDataBase->mimeTypeForFile(info).iconName());
    }
    return QIcon::fromTheme((mimeDataBase->mimeTypeForUrl(QUrl(info.filePath())).iconName()));
}
