#include "addactiondialog.h"
#include "ui_addactiondialog.h"

#include "fmiconview.h"
#include "splitter.h"
#include "titlebar.h"

#include <QDebug>

#include <QSettings>
#include <QStandardPaths>
#include <QDir>

AddActionDialog::AddActionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddActionDialog)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &AddActionDialog::addIconView);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &AddActionDialog::hide);
}

AddActionDialog::~AddActionDialog()
{
    delete ui;
}

void AddActionDialog::addIconView() {
    if (ui->lineEdit->text().isEmpty()) {
        return;
    }
    QSettings idCount;
    idCount.beginGroup("iconview/id_count");
    int lastViewId = idCount.value("lastid").toInt();
    qDebug()<<"lastid: "<<lastViewId;
    //add iconview with mkdir id+1.
    idCount.setValue("lastid", lastViewId+1);
    qDebug()<<idCount.value("lastid").toInt();
    idCount.endGroup();

    QSettings id_title_table;
    id_title_table.beginGroup("iconview/id_title_table");
    id_title_table.setValue(QString(lastViewId+1), ui->lineEdit->text()); //title may be changed, but id not.
    id_title_table.endGroup();

    //TODO: show active window only.
    QSettings id_state_table;
    id_state_table.beginGroup("iconview/id_state_table"); //this is sure whether iconview id is active.
    id_state_table.setValue(QString(lastViewId+1), 1); //'1' is active, '0' is inactive.
    id_state_table.endGroup();

    QDir test;
    test.setCurrent(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
    qDebug()<<test.mkpath(QString(".iconviews_on_desktop_test/")+QString::number(lastViewId+1));

    Splitter *page = new Splitter(lastViewId+1, ui->lineEdit->text());
    page->setOrientation(Qt::Vertical);
    TitleBar *bar = new TitleBar(page);
    bar->setTitle(ui->lineEdit->text());
    qDebug()<<ui->lineEdit->text();

    /* NOTE:
     * if we done the dialog here, the progress will crashed...
     * and i don't know why...
     * maybe we need to use signal/slot to emit a signal, let other object
     * handle the show event, so that we can done the dialog.
     */
    FMIconView *iconview = new FMIconView(lastViewId+1);
    page->addWidget(bar);
    page->setTitleBar(bar);
    bar->setIconView(iconview);
    page->addWidget(iconview);
    page->show();

    //this->done(1); //let the progress crash
    this->hide();
}
