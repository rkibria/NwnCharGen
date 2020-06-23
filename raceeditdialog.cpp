#include "raceeditdialog.h"
#include "ui_raceeditdialog.h"

#include <Nwn/rules.hpp>
#include <Nwn/race.hpp>
#include <Nwn/ablblock.hpp>

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
