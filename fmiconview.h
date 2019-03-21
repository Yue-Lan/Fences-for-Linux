#ifndef FMICONVIEW_H
#define FMICONVIEW_H

/*
QT_BEGIN_NAMESPACE
class QDragEnterEvent;
class QDropEvent;
QT_END_NAMESPACE
*/

#include <QObject>
#include <QWidget>
#include <QListView>
#include <QFileSystemModel>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QString>
#include <QSystemTrayIcon>
#include <QListWidget>
#include <QEvent>
#include <QCloseEvent>

#include <QString>

#include <QAction>

//#include <QSettings>

class FMIconView : public QListView
{
public:
    FMIconView(int id);
    void setViewPath(QString path);
    void writeSettings();
    void readSettings();

private:
    ~FMIconView();
    QFileSystemModel *fileSystemModel;
    QItemSelectionModel *itemSelectionModel;

    //QDragEnterEvent *currentDragEnterEvent;
    //QDropEvent *currentDropEvent;

    QMenu *blankMenu;
    QMenu *oneSelectedMenu;
    QMenu *pluralitySelectedMenu;

    QAction *pasteAction;
    QAction *updirAction;

    int mId = -1;

    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;
    void paintEvent(QPaintEvent *e) override;
    void closeEvent(QCloseEvent *e) override;

    void focusOutEvent(QFocusEvent *e);

    void moveSelectionToDesktop();
    void copySelectionToClipboard();
    void deleteSelection();
    void pasteFilesInClipboard();

Q_SIGNALS:
    void test();

protected:
    void dragEnterEvent(QDragEnterEvent *e) override;
    void dragMoveEvent(QDragMoveEvent *e) override;
    void dropEvent(QDropEvent *e) override;

public Q_SLOTS:
    void trayIconAction(QSystemTrayIcon::ActivationReason reason);
    void openFileOrDirectory(QModelIndex m);
    void showMenu();
    void backToUp();
};

#endif // FMICONVIEW_H
