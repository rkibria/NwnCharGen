#include <QInputDialog>
#include <QMessageBox>

#include "featdialog.h"
#include "ui_featdialog.h"

#include <Nwn/race.hpp>
#include <Nwn/rules.hpp>

#include <unordered_map>

using namespace Nwn;

FeatDialog::FeatDialog( Nwn::Rules *rules, bool choiceOnly, QWidget *parent ) :
    QDialog( parent ),
    ui( new Ui::FeatDialog ),
    nwnRules{ rules }
{
    ui->setupUi( this );

    setupWidget();
}

FeatDialog::~FeatDialog()
{
    delete ui;
}

void FeatDialog::setupWidget()
{
}
