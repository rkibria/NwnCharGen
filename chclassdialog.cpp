#include "chclassdialog.h"
#include "ui_chclassdialog.h"

ChClassDialog::ChClassDialog( Nwn::Rules *rules, Nwn::Character *nwnchar, QWidget *parent ) :
    QDialog(parent),
    ui(new Ui::ChClassDialog),
    nwnRules( rules ),
    nwnChar( nwnchar )
{
    ui->setupUi(this);
}

ChClassDialog::~ChClassDialog()
{
    delete ui;
}
