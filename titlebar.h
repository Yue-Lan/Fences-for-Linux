#ifndef TITLE_BAR
#define TITLE_BAR

#include "fmiconview.h"

#include <QWidget>
#include <QPoint>
#include <QDragMoveEvent>
#include <QDragEnterEvent>
#include <QRect>

class QLabel;
class QPushButton;

class TitleBar : public QWidget
{
    Q_OBJECT

public:
    explicit TitleBar(QWidget *parent = 0);
    void setIconView(FMIconView *iconview);
    void setTitle(QString title);
    ~TitleBar();

protected:

    // 双击标题栏进行界面的最大化/还原
    virtual void mouseDoubleClickEvent(QMouseEvent *event);

    // 进行鼠界面的拖动
    virtual void mousePressEvent(QMouseEvent *event);

    virtual void mouseMoveEvent(QMouseEvent *event);

    // 设置界面标题与图标
    virtual bool eventFilter(QObject *obj, QEvent *event);

private Q_SLOTS:

    // 进行最小化、最大化/还原、关闭操作
    void onClicked();

private:

    // 最大化/还原
    void updateMaximize();

private:
    QLabel *m_pIconLabel;
    QLabel *m_pTitleLabel;
    QPushButton *m_pMinimizeButton;
    QPushButton *m_pMaximizeButton;
    QPushButton *m_pCloseButton;

    QPoint startPos;
    QPoint clickPos;

    bool is_pressed=false;
    FMIconView *mIconView;
    QRect currentRect;
};

#endif // TITLE_BAR
