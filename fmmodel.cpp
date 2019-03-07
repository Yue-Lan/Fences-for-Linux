#include "fmmodel.h"
#include <QIcon>
#include <QFileInfo>
#include <QMimeType>
#include <QDebug>

FMModel::FMModel()
{
    mimeDataBase = new QMimeDatabase;
}

QIcon FMModel::fileIcon(const QModelIndex &index) const{
    qDebug()<<(mimeDataBase->mimeTypeForFile(this->fileInfo(index))).iconName();
    return QIcon::fromTheme((mimeDataBase->mimeTypeForFile(this->fileInfo(index))).iconName());
}
