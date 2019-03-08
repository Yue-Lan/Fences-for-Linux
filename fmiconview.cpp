#include "fmiconview.h"
#include "fmmodel.h"
#include "fmfileiconprovider.h"

//#include "titlebar.h"

#include <QSettings>

#include <QDebug>
#include <QMimeData>
#include <QUrl>
#include <QList>

#include <QPalette>

#include <QtX11Extras/QX11Info>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#include <QDrag>
#include <QDropEvent>

#include <QDesktopServices>
#include <QApplication>

#include <QMenu>
#include <QAction>

#include <QListWidgetItem>
#include <QFileIconProvider>
#include <QIcon>

#include <QPalette>
#include <QKeyEvent>

#include <QPainter>
#include <QPaintEvent>

#include <QVBoxLayout>

#include <QApplication>
#include <QClipboard>
#include <QMimeData>

FMIconView::~FMIconView(){

    delete fileSystemModel;
    delete itemSelectionModel;
    delete blankMenu;
    delete oneSelectedMenu;
    delete pluralitySelectedMenu;

}

void FMIconView::closeEvent(QCloseEvent *e) {
    //writeSettings();
    QListView::closeEvent(e);
}

FMIconView::FMIconView(int id)
{
    mId = id;
    setAutoFillBackground(true);
    FMModel *model = new FMModel;
    //QFileSystemModel *model = new QFileSystemModel;

    FMFileIconProvider *fileIconProvider = new FMFileIconProvider;
    model->setIconProvider(fileIconProvider);

    //setAttribute(Qt::WA_TranslucentBackground,true);

    /*********************transparent test********************************/
    //setAttribute(Qt::WA_TranslucentBackground, true);
    //setWindowState (Qt::WindowMaximized);

    //QPalette pal = palette();
    //pal.setColor(QPalette::Foreground, QColor(0x00,0xff,0x00,0x00));
    //setPalette(pal);
    //setFixedSize(600,600);
    //setWindowOpacity(0.5);
    //move(1000,200);
    //setStyleSheet("background-color:transparent,white");
    //setStyleSheet("color:white;background-color:rgb(0,0,0)");
    //setStyleSheet("background-color:rgba(200,200,200,0.6)");
    //setStyleSheet("background-color:rgba(244,244,244,2)");

    setStyleSheet("color: white;"
                  "gridline-color: black;"
                  //"{"
                  "background-color: rgba(100, 100, 100, 0.5);"
                  //"opacity:0.5;"
                  //"}"
                  //"background-color: transparent;"
                  "alternate-background-color: rgba(64, 64, 64, 0.5);"
                  "selection-color: white;"
                  //"selection-background-color: rgba(77, 77, 77, 0.5);"
                  "border: 3px groove gray;"
                  "border-radius: 0px;"
                  "font-size:16px;"
                  //"word-wrap: break-word;"
                  //"break-word:normal;"
                  "padding: 2px 4px;");


    /*********************transparent test********************************/

    //model->setRootPath("/");
    model->setRootPath(QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"/.iconviews_on_desktop_test/"+QString::number(id));
    setModel(model);

    QItemSelectionModel *selections = new QItemSelectionModel(model);
    setViewMode(QListView::IconMode);
    setRootIndex(model->index(QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"/.iconviews_on_desktop_test/"+QString::number(id)));
    //setRootIndex(model->index("/"));
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setGridSize(QSize(108,108));
    //setIconSize(QSize(48,48));

    setDragEnabled(true);
    //setDropIndicatorShown(true);

    setAcceptDrops(true);
    //setDefaultDropAction(Qt::MoveAction);
    setDragDropOverwriteMode(true);

    setDragDropMode(QListView::DragDrop);
    setResizeMode(QListView::Adjust);

    connect(this, &FMIconView::doubleClicked, this, &FMIconView::openFileOrDirectory);
    qDebug()<<QApplication::desktopFileName();

    setContextMenuPolicy(Qt::CustomContextMenu);
    blankMenu = new QMenu;

    QAction *updir = new QAction(blankMenu);
    updirAction = updir;
    updir->setText("up");
    connect(updir, &QAction::triggered, this, &FMIconView::backToUp);

    QAction *paste = new QAction(blankMenu);
    pasteAction = paste;
    paste->setText("paste");
    connect(paste, &QAction::triggered, this, &FMIconView::pasteFilesInClipboard);

    blankMenu->addAction(updir);
    blankMenu->addAction(paste);

    oneSelectedMenu = new QMenu;
    QAction *moveToDesktopAction = new QAction(oneSelectedMenu);
    moveToDesktopAction->setText("move to desktop");
    connect(moveToDesktopAction, &QAction::triggered, this, &FMIconView::moveSelectionToDesktop);

    QAction *copyAction = new QAction(oneSelectedMenu);
    copyAction->setText("copy to clipboard");
    connect(copyAction, &QAction::triggered, this, &FMIconView::copySelectionToClipboard);

    QAction *deleteAction = new QAction(oneSelectedMenu);
    deleteAction->setText("delete, not to trash");
    connect(deleteAction, &QAction::triggered, this, &FMIconView::deleteSelection);

    oneSelectedMenu->addAction(moveToDesktopAction);
    oneSelectedMenu->addAction(copyAction);
    oneSelectedMenu->addAction(deleteAction);

    //TODO: add pluralitySelectedMenu actions
    pluralitySelectedMenu = new QMenu;

    //connect(this, &FMIconView::customContextMenuRequested, this, &FMIconView::showMenu);


    fileSystemModel = model;
    itemSelectionModel = selections;
    //setEditTriggers(QAbstractItemView::AnyKeyPressed);
    //setTextElideMode();
    //setFocusPolicy(Qt::StrongFocus);
    //keyPressEvent();
    //setFlow(QListView::TopToBottom);
    //setSpacing(100);
    //setTextElideMode(Qt::ElideMiddle);
    setUniformItemSizes(true);
    setMovement(QListView::Snap);
}

void FMIconView::dragEnterEvent(QDragEnterEvent *e){
    qDebug()<<"drag enter event";
    if (e->mimeData()->hasUrls()){
        QList<QUrl> urls = e->mimeData()->urls();
        qDebug()<<urls<<e->mimeData()->formats();
        if (e->source()==this){
            e->setDropAction(Qt::MoveAction);
            e->accept();
        } else {
            e->acceptProposedAction();
        }
    } else {
        e->ignore();
    }
    //QListView::dragEnterEvent(e);
}

/* NOTE:
 * we must override the dragMoveEvent(),
 * because the default event would ignore the drag event
 * if which is from other widgets,
 * and drop event could not happend.
 */
void FMIconView::dragMoveEvent(QDragMoveEvent *e) {
    e->accept();

    QModelIndex index = indexAt(e->pos());
    if (index.isValid()) {
        //qDebug()<<e->pos()<<index.model()->itemData(index);
        //qDebug()<<fileSystemModel->filePath(index);
        //highlight the index when drag move at.
    }
}

void FMIconView::dropEvent(QDropEvent *e){
    qDebug()<<"drop event";

    /* NOTE:
     * the QModeIndex var index presents the index that current file mouse hanging on.
     * we need to consider under what circumstances to perform the files move operation.
     */
    QModelIndex index = indexAt(e->pos());
    qDebug()<<fileSystemModel->filePath(index);
    if (this==(e->source())){
        //TODO: when drop uppon to a directory icon, move selection in to this.
        if (index.isValid()) {
            QUrl tmpUrl = QUrl("file://"+fileSystemModel->filePath(index));
            if (!e->mimeData()->urls().contains(tmpUrl)&&fileSystemModel->fileInfo(index).isDir()){
                e->setDropAction(Qt::MoveAction);
                qDebug()<<"files move action";
                //May be we need a thread job to handle lots of files move action?
            }
            e->accept();
        } else {
            e->ignore();
        }
        return;
    } else {
        //drag form other widget
        QUrl tmpUrl = QUrl("file://"+fileSystemModel->filePath(index));
        qDebug()<<tmpUrl;
        if (e->mimeData()->hasUrls()){
            QList<QUrl> urls = e->mimeData()->urls();
            qDebug()<<urls;
            /* NOTE:
             * when accept drop from other widget, we aslo need consider how to deal with the files opreation.
             * probably there is a better way to handle drop event.
             */
            //QUrl tmpUrl = QUrl("file://"+fileSystemModel->fileName(index));
            if (!index.isValid()) {
                QUrl currentDirUrl = QUrl("file://"+fileSystemModel->filePath(this->rootIndex()));
                qDebug()<<currentDirUrl<<endl<<"!valid";
                if (!e->mimeData()->urls().contains(currentDirUrl)) {
                    e->setDropAction(Qt::MoveAction);
                    qDebug()<<"move files to current dir";
                    for(int i = 0; i<urls.count(); i++) {
                        QUrl tmpUrl = urls.at(i);
                        qDebug()<<"tmpUrl: "<<tmpUrl;
                        QFileInfo tmpInfo = QFileInfo(tmpUrl.path());
                        QString filename = tmpInfo.fileName();
                        qDebug()<<filename;
                        qDebug()<<QString(QString(".iconviews_on_desktop_test/")+QString::number(mId)+QString("/")+filename);
                        QString sourcePath = tmpInfo.filePath();
                        //this is same to fileSystemModel->filePath(this->rootIndex()) here, and more fast.
                        QString destPath = QString(QString(QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"/.iconviews_on_desktop_test/")+QString::number(mId)+QString("/")+filename);
                        if (tmpInfo.isFile()) {
                            qDebug()<<"is file";
                            qDebug()<<QFile::rename(sourcePath, destPath);
                        } else if (tmpInfo.isDir()) {
                            qDebug()<<"is dir";
                            qDebug()<<QDir(tmpInfo.filePath()).rename(sourcePath, destPath);
                        } else {
                            qDebug()<<"tmpInfo has not type?";
                        }
                    }
                    //May be we need a thread job to handle lots of files move action?
                } else {
                    qDebug()<<"warning dialog, can't copy/move dir into itself";
                }
            } else if (!e->mimeData()->urls().contains(tmpUrl)&&fileSystemModel->fileInfo(index).isDir()) {
                e->setDropAction(Qt::MoveAction);
                qDebug()<<"files move action";
                //May be we need a thread job to handle lots of files move action?
            } else {
                e->setDropAction(Qt::CopyAction);
                qDebug()<<"files copy action";
                //May be we need a thread job to handle lots of files move action?
            }
            e->acceptProposedAction();
            QListView::dropEvent(e);  //this may be useless.
        } else {
            e->ignore();
        }
    }
}

void FMIconView::trayIconAction(QSystemTrayIcon::ActivationReason reason){
    if (reason){

    }
    qDebug()<<"trayIconAction";
}

void FMIconView::openFileOrDirectory(QModelIndex m){
    qDebug()<<m<<fileSystemModel->fileInfo(m);
    if (fileSystemModel->isDir(m)){
        setRootIndex(m);
        //QDesktopServices::openUrl(QUrl("file://"+fileSystemModel->filePath(m)));
    } else {
        QDesktopServices::openUrl(QUrl("file://"+fileSystemModel->filePath(m)));
    }
}

void FMIconView::showMenu(){
    blankMenu->exec(QCursor::pos());
}

void FMIconView::backToUp(){
    qDebug()<<"backToUp?";
    if (rootIndex().parent().isValid()) {
        qDebug()<<"backToUp";
        setRootIndex(rootIndex().parent());
    }
}

void FMIconView::mouseReleaseEvent(QMouseEvent *e){
    qDebug()<<"mouseReleaseEvent";

    if (fileSystemModel->filePath(rootIndex()) == fileSystemModel->rootPath()) {
        updirAction->setEnabled(false);
    } else {
        updirAction->setEnabled(true);
    }
    if (e->button()==Qt::RightButton) {
        if (!this->indexAt(e->pos()).isValid()) {
            if (!QApplication::clipboard()->mimeData()->hasUrls()) {
                pasteAction->setEnabled(false);
            } else {
                pasteAction->setEnabled(true);
            }
            blankMenu->exec(QCursor::pos());
        } else {
            if (this->selectedIndexes().count()==1) {
                oneSelectedMenu->exec(QCursor::pos());
            } else {
                //TODO: use pluralitySelectedMenu here
                oneSelectedMenu->exec(QCursor::pos());
            }
        }
    }

    QListView::mouseReleaseEvent(e);
}

void FMIconView::mouseMoveEvent(QMouseEvent *e) {
    QListView::mouseMoveEvent(e);
}

void FMIconView::keyReleaseEvent(QKeyEvent *e){
    qDebug()<<"keyReleaseEvent";
    QListView::keyReleaseEvent(e);
}

void FMIconView::paintEvent(QPaintEvent *e){
    /*
    QPainter p(this);
    p.setCompositionMode(QPainter::CompositionMode_Clear);
    p.fillRect( 10, 10, 300, 300, Qt::SolidPattern );
    */
    QListView::paintEvent(e);
}

void FMIconView::moveSelectionToDesktop() {
    QModelIndexList selectionList = this->selectedIndexes();
    for (int i = 0; i < selectionList.count(); i++) {
        QModelIndex tmpIndex = selectionList.at(i);
        QString sourcePath = fileSystemModel->filePath(tmpIndex);
        QString destPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + QString("/") + fileSystemModel->fileName(tmpIndex);
        if (fileSystemModel->isDir(tmpIndex)) {
            QDir dirHelper;
            dirHelper.rename(sourcePath, destPath);
        } else {
            QFile fileHelper;
            fileHelper.rename(sourcePath, destPath);
        }
    }
}

void FMIconView::copySelectionToClipboard() {
    QModelIndexList selectionList = this->selectedIndexes();
    QMimeData *selectionMimeData = fileSystemModel->mimeData(selectionList);
    QApplication::clipboard()->setMimeData(selectionMimeData);
}

void FMIconView::deleteSelection() {
    QModelIndexList selectionList = this->selectedIndexes();
    for (int i = 0; i < selectionList.count(); i++) {
        QModelIndex tmpIndex = selectionList.at(i);
        QString sourcePath = fileSystemModel->filePath(tmpIndex);
        if (fileSystemModel->isDir(tmpIndex)) {
            QDir dirHelper;
            dirHelper.setPath(sourcePath);
            dirHelper.removeRecursively();
        } else {
            QFile fileHelper;
            fileHelper.remove(sourcePath);
        }
    }
}

//TODO: we need support over one widget, so every settings should have a diffrent name.
void FMIconView::readSettings() {
    /*
    QSettings settings;
    settings.beginGroup("my_listview1");
    restoreGeometry(settings.value("geometry").toByteArray());
    qDebug()<<settings.value("geometry");
    settings.endGroup();
    */
}

void FMIconView::writeSettings() {
    /*
    qDebug()<<"write settings";
    QSettings settings;
    settings.beginGroup("my_listview1");
    settings.setValue("geometry",saveGeometry());
    settings.endGroup();
    */
}

bool copyRecursively(const QString &srcFilePath, const QString &tgtFilePath)
{
    QFileInfo srcFileInfo(srcFilePath);
    if (srcFileInfo.isDir()) {
        QDir targetDir(tgtFilePath);
        bool ok = targetDir.cdUp();
        if (ok && !targetDir.mkdir(QFileInfo(tgtFilePath).fileName())) return false;

        QDir sourceDir(srcFilePath);
        QStringList fileNames = sourceDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);
        foreach(const QString &fileName, fileNames) {
            const QString newSrcFilePath = srcFilePath + QLatin1Char('/') + fileName;
            const QString newTgtFilePath = tgtFilePath + QLatin1Char('/') + fileName;
            if (!copyRecursively(newSrcFilePath, newTgtFilePath)) return false;
        }
    }
    else {
        if (!QFile::copy(srcFilePath, tgtFilePath)) return false;
    }

    return true;
}

void FMIconView::pasteFilesInClipboard() {
    qDebug()<<"pasteFilesInClipboard";
    QList<QUrl> urls = QApplication::clipboard()->mimeData()->urls();
    qDebug()<<urls;
    for(int i = 0 ; i<urls.count(); i++) {
        QUrl tmpUrl = urls.at(i);
        qDebug()<<"pasteFilesInClipboard tmpUrl: "<<tmpUrl;
        QFileInfo tmpInfo = QFileInfo(tmpUrl.path());
        QString filename = tmpInfo.fileName();
        qDebug()<<filename;
        qDebug()<<QString(QString(".iconviews_on_desktop_test/")+QString::number(mId)+QString("/")+filename);
        QString sourcePath = tmpInfo.filePath();
        //this is same to fileSystemModel->filePath(this->rootIndex()) here, and more fast.
        QString destPath = QString(QString(QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"/.iconviews_on_desktop_test/")+QString::number(mId)+QString("/")+filename);
        copyRecursively(sourcePath, destPath);
    }
}

void FMIconView::focusOutEvent(QFocusEvent *e) {
    if (blankMenu->isHidden() && oneSelectedMenu->isHidden() && pluralitySelectedMenu->isHidden()){
        this->clearSelection();
    }
    QListView::focusOutEvent(e);
}

