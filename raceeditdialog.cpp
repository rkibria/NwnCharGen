#include "raceeditdialog.h"
#include "ui_raceeditdialog.h"

RaceEditDialog::RaceEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RaceEditDialog)
{
    ui->setupUi(this);
}

RaceEditDialog::~RaceEditDialog()
{
    delete ui;
}
