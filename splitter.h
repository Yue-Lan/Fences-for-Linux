#ifndef SPLITTER_H
#define SPLITTER_H

#include <QObject>
#include <QWidget>
#include <QSplitter>
#include <QCloseEvent>
#include <QtGui>

#include <QString>

#include <QMouseEvent>

#include "titlebar.h"

class Splitter : public QSplitter
{
public:
    Splitter(int id, QString title);
    void setTitleBar (TitleBar* bar);
    void readSettings();
    void writeSettings();

    int countFlag(QPoint p, int row);
    void setCursorType(int flag);
    int countRow(QPoint p);

protected:
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);

private:
    void closeEvent(QCloseEvent *e) override;
    bool isLeftPressed;
    int curPos;
    QPoint pLast;

    TitleBar *mTitleBar;
    int mId;
    QString mTitle;
};

#endif // SPLITTER_H
