#ifndef FMMODEL_H
#define FMMODEL_H

#include <QObject>
#include <QWidget>
#include <QFileSystemModel>
#include <QItemDelegate>
#include <QMimeDatabase>

class FMModel : public QFileSystemModel
{
public:
    FMModel();
    QIcon fileIcon(const QModelIndex &index) const;

private:
    QMimeDatabase *mimeDataBase;
};

#endif // FMMODEL_H
