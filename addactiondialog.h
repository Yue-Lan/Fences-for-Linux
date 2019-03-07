#ifndef ADDACTIONDIALOG_H
#define ADDACTIONDIALOG_H

#include <QDialog>

namespace Ui {
class AddActionDialog;
}

class AddActionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddActionDialog(QWidget *parent = 0);
    ~AddActionDialog();

private:
    Ui::AddActionDialog *ui;

private slots:
    void addIconView();
};

#endif // ADDACTIONDIALOG_H
