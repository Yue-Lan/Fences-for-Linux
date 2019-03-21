#include "fmfileiconprovider.h"

#include <QFileInfo>
#include <QPixmap>
#include <QFile>

#include <QUrl>
#include <QDebug>
#include <QSettings>
#include <QDesktopServices>

#include <gio/gio.h>

FMFileIconProvider::FMFileIconProvider()
{
    mimeDataBase = new QMimeDatabase;
}

FMFileIconProvider::~FMFileIconProvider() {
    delete &mimeDataBase;
}

void FMFileIconProvider::setIconSize(int size) {
    if (size > 0)
        mIconSize = size;
}

//Qt can't get theme icon directly. use QIcon::fromTheme can, but we need setThemeName first.
QIcon FMFileIconProvider::icon(const QFileInfo &info) const {
/*
    const QString filePath = info.filePath();
    std::string str = filePath.toStdString();
    const char* file_path = str.c_str();
    qDebug()<<file_path;
    GFile *g_file = g_file_new_for_path(file_path);
    GFileInfo *file_info = g_file_query_info(g_file,
                                             "standard::*",
                                             G_FILE_QUERY_INFO_NONE,
                                             NULL,
                                             NULL);
    GIcon *g_icon = g_file_info_get_icon (file_info);
    const gchar* const* icon_names = g_themed_icon_get_names(G_THEMED_ICON (g_icon));
    qDebug()<<*icon_names;
    qDebug()<<g_icon_to_string(g_icon);

    QString iconName = QString (*icon_names);
    qDebug()<<iconName;

    g_object_unref(g_icon);
    g_object_unref(g_file);

    //return QIcon::fromTheme(iconName);
*/
    //image file, return a thumbnail.
    QPixmap tmpPixmap = QPixmap(info.filePath());
    if (!tmpPixmap.isNull()){
        tmpPixmap = tmpPixmap.scaled(mIconSize, mIconSize);
        return QIcon(tmpPixmap);
    }

    //this is not a good way.
    if (mimeDataBase->mimeTypeForFile(info).iconName() == QString("application-x-desktop")) {
        qDebug()<<"is .desktop file";
        QSettings desktopFile(info.filePath(), QSettings::IniFormat);
        desktopFile.setIniCodec("UTF-8");
        QPixmap desktopFileIconPixmap = QPixmap(desktopFile.value("Desktop Entry/Icon").toString());
        if (!desktopFileIconPixmap.isNull()) {
            desktopFileIconPixmap = desktopFileIconPixmap.scaled(mIconSize,mIconSize);
            return QIcon(desktopFileIconPixmap);
        }
        QIcon icon = QIcon::fromTheme(desktopFile.value("Desktop Entry/Icon").toString());
        qDebug()<<icon.name();
        return icon;
    }

    if (mimeDataBase->mimeTypeForUrl(QUrl(info.filePath())).aliases().isEmpty()) {
        return QIcon::fromTheme(mimeDataBase->mimeTypeForFile(info).iconName());
    }
    return QIcon::fromTheme((mimeDataBase->mimeTypeForUrl(QUrl(info.filePath())).iconName()));
}
