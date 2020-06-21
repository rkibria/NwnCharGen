#include "rulesracesdialog.h"
#include "ui_rulesracesdialog.h"

RulesRacesDialog::RulesRacesDialog( const Nwn::Rules *rules, QWidget *parent ) :
    QDialog( parent ),
    ui( new Ui::RulesRacesDialog ),
    nwnRules( rules )
{
    ui->setupUi( this );
}

RulesRacesDialog::~RulesRacesDialog()
{
    delete ui;
}
