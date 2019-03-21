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
    void setIconSize(int size);
    QIcon icon(const QFileInfo &info) const;

private:
    QMimeDatabase *mimeDataBase;
    int mIconSize = 36;
    ~FMFileIconProvider();
};

#endif // FMFILEICONPROVIDER_H
