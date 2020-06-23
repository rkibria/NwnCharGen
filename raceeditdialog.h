#ifndef RACEEDITDIALOG_H
#define RACEEDITDIALOG_H

#include <QDialog>

namespace Ui {
class RaceEditDialog;
}

class RaceEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RaceEditDialog(QWidget *parent = nullptr);
    ~RaceEditDialog();

private:
    Ui::RaceEditDialog *ui;
};

#endif // RACEEDITDIALOG_H
