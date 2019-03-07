#ifndef FMFILEICONPROVIDER_H
#define FMFILEICONPROVIDER_H

#include <QObject>
#include <QWidget>
#include <QFileIconProvider>
#include <QMimeDatabase>
#include <QIcon>

class FMFileIconProvider : public QFileIconProvider
{
public:
    FMFileIconProvider();
    QIcon icon(const QFileInfo &info) const;

private:
    QMimeDatabase *mimeDataBase;
};

#endif // FMFILEICONPROVIDER_H
