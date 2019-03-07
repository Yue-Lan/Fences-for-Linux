#include <QApplication>

#include <QIcon>
#include <QMenu>
#include <QAction>
#include <QApplication>

#include <QStandardPaths>
#include <QFileIconProvider>

#include <QSettings>

#include "fmiconview.h"
#include "splitter.h"
#include "titlebar.h"
#include "addactiondialog.h"
#include "settingscontroller.h"

#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //allReset();

    /* NOTE:
     * we need set system theme and inhert the QFileIconProvider's icon() method,
     * QIcon::fromTheme will return the theme icon, see fmfileiconprovider.cpp.
     * TODO:
     * get the current system icon theme
     * gsettings get org.mate.interface icon-theme
     * is there a more general way?
     */
    //QGSettings gsettings = QGSettings(QByteArray("org.mate.interface"),QByteArray("icon-theme"));
    //qDebug()<<gsettings.get("org.mate.interface icon-theme");

    QIcon::setThemeName("ukui-icon-theme");

//============================================================

    QSettings iconview_idcount;
    iconview_idcount.beginGroup("iconview/id_count");
    int idCout = iconview_idcount.value("lastid").toInt();
    for (int i = 1; i <= idCout; i++) {
        QSettings iconview_id_title_table;
        iconview_id_title_table.beginGroup("iconview/id_title_table");
        QString title_for_this_id = iconview_id_title_table.value(QString(i)).toString();
        qDebug()<<"id: "<<i<<", title: "<<title_for_this_id;

        QSettings iconview_id_state_table;
        iconview_id_state_table.beginGroup("iconview/id_state_table");
        int state = iconview_id_state_table.value(QString(i)).toInt();
        if (state == 1) {
            Splitter *page = new Splitter(i, title_for_this_id);
            page->setOrientation(Qt::Vertical);
            TitleBar *bar = new TitleBar(page);
            bar->setTitle(title_for_this_id);
            FMIconView *iconview = new FMIconView(i);
            page->addWidget(bar);
            page->setTitleBar(bar);
            bar->setIconView(iconview);
            page->addWidget(iconview);
            page->show();
        }
    }

    QSystemTrayIcon *tray = new QSystemTrayIcon;
    QFileIconProvider iconProvider;
    tray->setIcon(QIcon(iconProvider.icon(QFileIconProvider::Desktop)));
    tray->show();

    QMenu *trayMenu;
    trayMenu = new QMenu();
    QAction *trayQuitAction;
    trayQuitAction = new QAction();
    trayQuitAction->setText("save and quit");
    QObject::connect(trayQuitAction,SIGNAL(triggered()),qApp,SLOT(quit()));

    QAction *trayAddWidgetAction = new QAction;
    trayAddWidgetAction->setText("add");

    AddActionDialog *addActionDialog = new AddActionDialog;
    QObject::connect(trayAddWidgetAction, &QAction::triggered, addActionDialog, &AddActionDialog::show);

    QAction *trayResetAction = new QAction;
    trayResetAction->setText("delete all views and files and quit");

    SettingsController *controller = new SettingsController;
    QObject::connect(trayResetAction, &QAction::triggered, controller, &SettingsController::resetAll);

    trayMenu->addAction(trayAddWidgetAction);
    trayMenu->addAction(trayQuitAction);
    trayMenu->addAction(trayResetAction);
    tray->setContextMenu(trayMenu);

    return a.exec();
}
