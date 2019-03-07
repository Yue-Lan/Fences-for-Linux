#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QPixmap>

#include "titlebar.h"

#include <QDebug>
#include <QDrag>
#include <QMimeData>


void TitleBar::setTitle(QString title) {
    m_pTitleLabel->setText(title);
}

TitleBar::TitleBar(QWidget *parent)
    : QWidget(parent)
{
    setFixedHeight(24);

    m_pTitleLabel = new QLabel(this);
    //m_pMinimizeButton = new QPushButton(this);
    //m_pMinimizeButton->setIcon(QIcon(":/icons/hide.png"));
    m_pCloseButton = new QPushButton(this);
    m_pCloseButton->setIcon(QIcon(":/icons/delete.png"));

    m_pTitleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_pTitleLabel->setAlignment(Qt::AlignHCenter);

    //m_pMinimizeButton->setFixedSize(24, 24);
    m_pCloseButton->setFixedSize(24, 24);

    m_pTitleLabel->setObjectName("whiteLabel");
    //m_pMinimizeButton->setObjectName("minimizeButton");
    m_pCloseButton->setObjectName("closeButton");

    //m_pMinimizeButton->setToolTip("Minimize");
    m_pCloseButton->setToolTip("Close");

    QHBoxLayout *pLayout = new QHBoxLayout(this);
    //pLayout->addWidget(m_pMinimizeButton);
    pLayout->addWidget(m_pTitleLabel);
    pLayout->addWidget(m_pCloseButton);
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(pLayout);

    //connect(m_pMinimizeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    connect(m_pCloseButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));

}

TitleBar::~TitleBar()
{

}

void TitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void TitleBar::mouseMoveEvent(QMouseEvent *event) {
    if (true){
        qDebug()<<"titlebar move event";
        parentWidget()->move(event->globalPos()-clickPos);
        //is_pressed = false;
    }
    QWidget::mouseMoveEvent(event);
}

void TitleBar::mousePressEvent(QMouseEvent *event)
{
    qDebug()<<"mousePressEvent";
    //is_pressed = true;
    clickPos = event->pos();
    QWidget::mousePressEvent(event);
}

bool TitleBar::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type())
    {
    case QEvent::WindowTitleChange:
    {
        QWidget *pWidget = qobject_cast<QWidget *>(obj);
        if (pWidget)
        {
            m_pTitleLabel->setText(pWidget->windowTitle());
            return true;
        }
    }
    case QEvent::WindowStateChange:
    case QEvent::Resize:
        updateMaximize();
        return true;
    default:
        return QWidget::eventFilter(obj, event);
    }
    return QWidget::eventFilter(obj, event);
}

void TitleBar::onClicked()
{
    QPushButton *pButton = qobject_cast<QPushButton *>(sender());
    QWidget *pWindow = this->window();
    if (pWindow->isTopLevel())
    {
        if (pButton == m_pMinimizeButton)
        {
            if(mIconView != NULL) {

                currentRect = mIconView->rect();
                qDebug()<<currentRect;
                mIconView->hide();
            }
            //pWindow->showMinimized();
        }
        else if (pButton == m_pCloseButton)
        {
            pWindow->close();
        }
    }
}

void TitleBar::updateMaximize()
{

}

void TitleBar::setIconView(FMIconView *iconview) {
    mIconView = iconview;
}
