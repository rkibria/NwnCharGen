#include "raceeditdialog.h"
#include "ui_raceeditdialog.h"

#include <Nwn/rules.hpp>
#include <Nwn/race.hpp>
#include <Nwn/ablblock.hpp>

#include <QMessageBox>

using namespace Nwn;

RaceEditDialog::RaceEditDialog( Nwn::Rules *r, const std::string& n, QWidget *parent ) :
    QDialog(parent),
    ui(new Ui::RaceEditDialog),
    nwnRules{ r },
    raceName{ n },
    raceCopy{ std::make_unique<Race>( nwnRules->getRaceByName( raceName ) ) }
{
    ui->setupUi(this);

    updateAll();
}

RaceEditDialog::~RaceEditDialog()
{
    delete ui;
}

void RaceEditDialog::done( int r )
{
    if( QDialog::Accepted == r ) {
        if( !ui->lineEditName->text().isEmpty() && !ui->lineEditClassification->text().isEmpty() ) {
            QDialog::done(r);
            return;
        }
        else {
            QMessageBox::critical( this, "Error", "Name and classification must not be empty!" );
            return;
        }
    }
    else {
        QDialog::done(r);
        return;
    }
}

void RaceEditDialog::updateAll()
{
    ui->lineEditName->setText( raceCopy->getName().c_str() );
    ui->lineEditClassification->setText( raceCopy->getClassification().c_str() );
    ui->textEditDescription->setText( raceCopy->getDescription().c_str() );

    ui->spinBoxStr->setValue( raceCopy->getAblAdjusts().getAbl( AblScore::Str ) );
    ui->spinBoxDex->setValue( raceCopy->getAblAdjusts().getAbl( AblScore::Dex ) );
    ui->spinBoxCon->setValue( raceCopy->getAblAdjusts().getAbl( AblScore::Con ) );
    ui->spinBoxInt->setValue( raceCopy->getAblAdjusts().getAbl( AblScore::Int ) );
    ui->spinBoxWis->setValue( raceCopy->getAblAdjusts().getAbl( AblScore::Wis ) );
    ui->spinBoxCha->setValue( raceCopy->getAblAdjusts().getAbl( AblScore::Cha ) );
}

void RaceEditDialog::on_buttonBox_accepted()
{
    raceCopy->setName( ui->lineEditName->text().toStdString() );
    raceCopy->setClassification( ui->lineEditClassification->text().toStdString() );
    raceCopy->setDescription( ui->textEditDescription->toPlainText().toStdString() );

    auto& abls = raceCopy->getAblAdjusts();
    abls.setAbl( AblScore::Str, ui->spinBoxStr->value() );
    abls.setAbl( AblScore::Dex, ui->spinBoxDex->value() );
    abls.setAbl( AblScore::Con, ui->spinBoxCon->value() );
    abls.setAbl( AblScore::Int, ui->spinBoxInt->value() );
    abls.setAbl( AblScore::Wis, ui->spinBoxWis->value() );
    abls.setAbl( AblScore::Cha, ui->spinBoxCha->value() );
}
