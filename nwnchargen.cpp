#include "nwnchargen.h"
#include "./ui_nwnchargen.h"

#include <nwnchar.hpp>
#include <nwnablblock.hpp>
#include <nwnrace.hpp>
#include <nwnrules.hpp>

#include "racedialog.h"

using namespace Nwn;

void NwnCharGen::updateAbilityBlock()
{
    ui->lineEditPoints->setText( QString::number( nwnChar->getPointsRemain() ) );

    ui->lineEditStrAbs->setText( QString::number( nwnChar->getAbls().getAbl( AblScore::Str ) ) );
    ui->lineEditDexAbs->setText( QString::number( nwnChar->getAbls().getAbl( AblScore::Dex ) ) );
    ui->lineEditConAbs->setText( QString::number( nwnChar->getAbls().getAbl( AblScore::Con ) ) );
    ui->lineEditIntAbs->setText( QString::number( nwnChar->getAbls().getAbl( AblScore::Int ) ) );
    ui->lineEditWisAbs->setText( QString::number( nwnChar->getAbls().getAbl( AblScore::Wis ) ) );
    ui->lineEditChaAbs->setText( QString::number( nwnChar->getAbls().getAbl( AblScore::Cha ) ) );

    ui->lineEditStrMod->setText( QString::number( nwnChar->getAbls().getAblMod( AblScore::Str ) ) );
    ui->lineEditDexMod->setText( QString::number( nwnChar->getAbls().getAblMod( AblScore::Dex ) ) );
    ui->lineEditConMod->setText( QString::number( nwnChar->getAbls().getAblMod( AblScore::Con ) ) );
    ui->lineEditIntMod->setText( QString::number( nwnChar->getAbls().getAblMod( AblScore::Int ) ) );
    ui->lineEditWisMod->setText( QString::number( nwnChar->getAbls().getAblMod( AblScore::Wis ) ) );
    ui->lineEditChaMod->setText( QString::number( nwnChar->getAbls().getAblMod( AblScore::Cha ) ) );
}

void NwnCharGen::updateSummary()
{
    ui->lineEditRace->setText( nwnChar->getRace().c_str() );

    updateAbilityBlock();
}

NwnCharGen::NwnCharGen(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::NwnCharGen)
    , nwnChar( std::make_unique<Character>() )
    , nwnRules( std::make_unique<Rules>() )
{
    ui->setupUi(this);
    setCentralWidget(ui->tabWidget);

    Race human( "Human", "Human" );
    human.setDescription("Humans are the most adaptable of the common races. Short generations and a penchant for migration and conquest mean they are very physically diverse as well. Skin shades range from nearly black to very pale, hair from black to blond, and facial hair (for men) from sparse to thick. Humans are often unorthodox in their dress, sporting unusual hairstyles, fanciful clothes, tattoos, and the like.");
    nwnRules->addRace( human );

    Race goldDwarf( Race( "Gold Dwarf", "Dwarf" ) );
    goldDwarf.setDescription("The gold dwarves maintained their great kingdom in the Great Rift and managed to survive terrible wars against evil humanoids. While they practiced some magic, they never acquired the hubris that caused the downfall of some human nations. Confident and secure in their remote home, the gold dwarves gained a reputation for haughtiness and pride. Since the Thunder Blessing, many young gold dwarves have left the Great Rift and are exploring the rest of the region. The folk of other lands have learned that while some gold dwarves are aloof and suspicious, for the most part they are forthright warriors and shrewd traders.");
    goldDwarf.getAblAdjusts().setAbl( AblScore::Con, 2 );
    goldDwarf.getAblAdjusts().setAbl( AblScore::Dex, -2 );
    nwnRules->addRace( goldDwarf );

    nwnRules->addRace( Race( "Gray Dwarf", "Dwarf" ) );
    nwnRules->addRace( Race( "Drow", "Elf" ) );

    updateSummary();
}

NwnCharGen::~NwnCharGen()
{
    delete ui;
}

void NwnCharGen::on_pushButtonStrMinus_clicked()
{
    nwnChar->decAbl( AblScore::Str );
    updateAbilityBlock();
}

void NwnCharGen::on_pushButtonStrPlus_clicked()
{
    nwnChar->incAbl( AblScore::Str );
    updateAbilityBlock();
}

void NwnCharGen::on_pushButtonDexMinus_clicked()
{
    nwnChar->decAbl( AblScore::Dex );
    updateAbilityBlock();
}

void NwnCharGen::on_pushButtonDexPlus_clicked()
{
    nwnChar->incAbl( AblScore::Dex );
    updateAbilityBlock();
}

void NwnCharGen::on_pushButtonConMinus_clicked()
{
    nwnChar->decAbl( AblScore::Con );
    updateAbilityBlock();
}

void NwnCharGen::on_pushButtonConPlus_clicked()
{
    nwnChar->incAbl( AblScore::Con );
    updateAbilityBlock();
}

void NwnCharGen::on_pushButtonIntMinus_clicked()
{
    nwnChar->decAbl( AblScore::Int );
    updateAbilityBlock();
}

void NwnCharGen::on_pushButtonIntPlus_clicked()
{
    nwnChar->incAbl( AblScore::Int );
    updateAbilityBlock();
}

void NwnCharGen::on_pushButtonWisMinus_clicked()
{
    nwnChar->decAbl( AblScore::Wis );
    updateAbilityBlock();
}

void NwnCharGen::on_pushButtonWisPlus_clicked()
{
    nwnChar->incAbl( AblScore::Wis );
    updateAbilityBlock();
}

void NwnCharGen::on_pushButtonChaMinus_clicked()
{
    nwnChar->decAbl( AblScore::Cha );
    updateAbilityBlock();
}

void NwnCharGen::on_pushButtonChaPlus_clicked()
{
    nwnChar->incAbl( AblScore::Cha );
    updateAbilityBlock();
}

void NwnCharGen::on_buttonRace_clicked()
{
    RaceDialog rd(nwnRules, this);
    if( rd.exec() == QDialog::Accepted ) {
        if( !rd.raceChoice.isEmpty() ) {
            nwnChar->setRace( rd.raceChoice.toStdString() );
            updateSummary();
        }
    }
}
