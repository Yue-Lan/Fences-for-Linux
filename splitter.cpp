#include "splitter.h"

#include <QtX11Extras/QX11Info>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#include <QSettings>
#include <QDebug>
#include <QPoint>
#include <QCursor>
#include <QApplication>
#include <QSize>

#define MARGIN 10
#define MINWIDTH 200
#define MINHEIGHT 150

void Splitter::setTitleBar (TitleBar *bar) {
    mTitleBar = bar;
}

Splitter::Splitter(int id, QString title)
{
    mId = id;
    mTitle = title;
    //window flags

    setMouseTracking(true);

    setWindowFlags (Qt::Window
                    | Qt::FramelessWindowHint
                    | Qt::WindowStaysOnBottomHint
                    | Qt::Tool);

    //follow the current workspace

    Atom net_wm_state_sticky=XInternAtom (QX11Info::display(),
                                          "_NET_WM_STATE_STICKY",
                                          True);

    Atom net_wm_state = XInternAtom (QX11Info::display(),
                                     "_NET_WM_STATE",
                                     False);

    XChangeProperty (QX11Info::display(),
                     winId(),
                     net_wm_state,
                     XA_ATOM,
                     32,
                     PropModeAppend,
                     (unsigned char *)&net_wm_state_sticky,
                     1);

    readSettings();
    //showNormal();
}

void Splitter::readSettings() {
    QSettings settings;
    settings.beginGroup("iconview/id_geometry_table");
    restoreGeometry(settings.value("geometry"+QString::number(mId)).toByteArray());
    qDebug()<<settings.value("geometry"+QString::number(mId));
    qDebug()<<settings.allKeys()<<endl<<settings.childKeys()<<endl<<settings.children();
    settings.endGroup();
}

void Splitter::writeSettings() {
    QSettings settings;
    settings.beginGroup("iconview/id_geometry_table");
    settings.setValue("geometry"+QString::number(mId),saveGeometry());
    settings.endGroup();
}

void Splitter::closeEvent(QCloseEvent *e) {
    writeSettings();
    QSplitter::closeEvent(e);
}

void Splitter::mousePressEvent(QMouseEvent *e) {
    if(e->button() == Qt::LeftButton)
    {
        this->isLeftPressed = true;
        QPoint temp = e->globalPos();
        //qDebug()<<"Press globalpos: "<<temp<<"Press pos: "<<e->pos();
        pLast = temp;
        curPos = countFlag(e->pos(), countRow(e->pos()));

        if(curPos != 22) //移动窗口
        {
            if (mTitleBar != NULL) {
                mTitleBar->setEnabled(false);
            }
        }
        e->ignore();
    }
}

void Splitter::mouseMoveEvent(QMouseEvent *e) {
    //qDebug()<<"move event splitter";
    int poss = countFlag(e->pos(), countRow(e->pos()));
    setCursorType(poss);
    //是否左击
    if(isLeftPressed)
    {
        QPoint ptemp = e->globalPos();
        //ptemp = ptemp - pLast;
        if(curPos == 22) //移动窗口
        {
            //ptemp = ptemp + e->pos();
            //ptemp = ptemp + pos();
            //move(ptemp);
        }
        else
        {
            QRect wid = geometry();
            qDebug()<<wid;

            //改变窗口的大小
            switch(curPos)
            {
            //左上角
            case 11:
                //wid.setTopLeft(wid.topLeft() + ptemp);
                wid.setTopLeft(e->globalPos());
                break;
                //右上角
            case 13:
                wid.setTopRight(wid.topRight() + ptemp);
                wid.setTopRight(e->globalPos());
                break;
                //左下角
            case 31:
                //wid.setBottomLeft(wid.bottomLeft() + ptemp);
                wid.setBottomLeft(e->globalPos());
                break;
                //右下角
            case 33:
                //wid.setBottomRight(wid.bottomRight() + ptemp);
                wid.setBottomRight(e->globalPos());
                break;
                //中上角
            case 12:
                //wid.setTop(wid.top() + ptemp.y());
                wid.setTop(e->globalY());
                break;
                //中左角
            case 21:
                //wid.setLeft(wid.left() + ptemp.x());
                wid.setLeft(e->globalX());
                break;
                //中右角
            case 23:
                //wid.setRight(wid.right() + ptemp.x());
                wid.setRight(e->globalX());
                break;
                //中下角
            case 32:
                //wid.setBottom(wid.bottom() + ptemp.y());
                wid.setBottom(e->globalY());
                break;
            }
            setGeometry(wid);
            e->accept();
        }

        //更新位置
        pLast = e->globalPos();
    } else {
        QApplication::restoreOverrideCursor();
    }
    e->ignore();
}

void Splitter::mouseReleaseEvent(QMouseEvent *e) {
    writeSettings();
    if (mTitleBar!=NULL) {
        mTitleBar->setEnabled(true);
    }
    qDebug()<<"splitter mouse release";
    setCursor(Qt::ArrowCursor);
    isLeftPressed = false;
}

//计算鼠标在哪一行那一列
int Splitter::countFlag(QPoint p, int row)
{
    if(p.y() < MARGIN)
        return 10+row;
    else if(p.y() > this->height()-MARGIN)
        return 30+row;
    else
        return 20+row;
}

//根据鼠标所在位置改变鼠标指针形状
void Splitter::setCursorType(int flag)
{
    //Qt::CursorShape cursor;
    QCursor cursor;
    switch(flag)
    {
    case 11:
    case 33:
        cursor = Qt::SizeFDiagCursor;
        break;
    case 13:
    case 31:
        cursor = Qt::SizeBDiagCursor;
        break;
    case 21:
    case 23:
        cursor = Qt::SizeHorCursor;
        break;
    case 12:
    case 32:
        cursor = Qt::SizeVerCursor;
        break;
    default:
        //恢复鼠标指针形状
        QApplication::restoreOverrideCursor();
        break;
    }
    //qDebug()<<flag<<endl<<cursor;
    setCursor(cursor);
}

//计算在哪一列
int Splitter::countRow(QPoint p)
{
    return (p.x()<MARGIN) ? 1:( (p.x()>(this->width()-MARGIN) ? 3:2) );
}
