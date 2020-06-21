#include <QFileDialog>
#include <QMessageBox>

#include "nwnchargen.h"
#include "./ui_nwnchargen.h"

#include <Nwn/base.hpp>
#include <Nwn/character.hpp>
#include <Nwn/ablblock.hpp>
#include <Nwn/race.hpp>
#include <Nwn/rules.hpp>

#include "racedialog.h"

using namespace Nwn;

NwnCharGen::NwnCharGen(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::NwnCharGen)
    , nwnChar( std::make_unique<Character>() )
    , nwnRules( std::make_unique<Rules>() )
    , dirtyFlag{ false }
    , currentFile{ "character.xml" }
    , currentRules()
{
    currentRules = "nwn2.xml";

    ui->setupUi(this);
    setCentralWidget(ui->tabWidget);

    for( size_t i = 0; i < alignmentStrings.size(); ++i ) {
        const auto a = static_cast<Alignment>( i );
        ui->comboBoxAlignment->addItem( alignmentStrings.left.at( a ).c_str() );
    }

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
    clearDirtyFlag();
}

NwnCharGen::~NwnCharGen()
{
    delete ui;
}

void NwnCharGen::updateAbilityBlock()
{
    ui->lineEditPoints->setText( QString::number( nwnChar->getPointsRemain() ) );

    const auto abls = nwnRules->getAdjustedAbls( *nwnChar );
    ui->lineEditStrAbs->setText( QString::number( abls.getAbl( AblScore::Str ) ) );
    ui->lineEditDexAbs->setText( QString::number( abls.getAbl( AblScore::Dex ) ) );
    ui->lineEditConAbs->setText( QString::number( abls.getAbl( AblScore::Con ) ) );
    ui->lineEditIntAbs->setText( QString::number( abls.getAbl( AblScore::Int ) ) );
    ui->lineEditWisAbs->setText( QString::number( abls.getAbl( AblScore::Wis ) ) );
    ui->lineEditChaAbs->setText( QString::number( abls.getAbl( AblScore::Cha ) ) );

    ui->lineEditStrMod->setText( QString::number( abls.getAblMod( AblScore::Str ) ) );
    ui->lineEditDexMod->setText( QString::number( abls.getAblMod( AblScore::Dex ) ) );
    ui->lineEditConMod->setText( QString::number( abls.getAblMod( AblScore::Con ) ) );
    ui->lineEditIntMod->setText( QString::number( abls.getAblMod( AblScore::Int ) ) );
    ui->lineEditWisMod->setText( QString::number( abls.getAblMod( AblScore::Wis ) ) );
    ui->lineEditChaMod->setText( QString::number( abls.getAblMod( AblScore::Cha ) ) );
}

void NwnCharGen::updateSummary()
{
    ui->lineEditName->setText( nwnChar->getName().c_str() );
    ui->textEditDescription->setText( nwnChar->getDescription().c_str() );
    ui->lineEditRace->setText( nwnChar->getRace().c_str() );
    ui->comboBoxAlignment->setCurrentIndex( static_cast<int>( nwnChar->getAlignment() ) );

    updateAbilityBlock();
}

void NwnCharGen::updateAll()
{
    updateSummary();
}

void NwnCharGen::modAbility( int iAbl, bool isInc )
{
    const auto abl = static_cast<AblScore>( iAbl );
    isInc ? nwnChar->incAbl( abl ) : nwnChar->decAbl( abl );
    setDirtyFlag();
    updateAbilityBlock();
}

void NwnCharGen::on_pushButtonStrMinus_clicked() { modAbility( static_cast<int>( AblScore::Str ), false ); }
void NwnCharGen::on_pushButtonStrPlus_clicked() { modAbility( static_cast<int>( AblScore::Str ), true ); }
void NwnCharGen::on_pushButtonDexMinus_clicked() { modAbility( static_cast<int>( AblScore::Dex ), false ); }
void NwnCharGen::on_pushButtonDexPlus_clicked() { modAbility( static_cast<int>( AblScore::Dex ), true ); }
void NwnCharGen::on_pushButtonConMinus_clicked() { modAbility( static_cast<int>( AblScore::Con ), false ); }
void NwnCharGen::on_pushButtonConPlus_clicked() { modAbility( static_cast<int>( AblScore::Con ), true ); }
void NwnCharGen::on_pushButtonIntMinus_clicked() { modAbility( static_cast<int>( AblScore::Int ), false ); }
void NwnCharGen::on_pushButtonIntPlus_clicked() { modAbility( static_cast<int>( AblScore::Int ), true ); }
void NwnCharGen::on_pushButtonWisMinus_clicked() { modAbility( static_cast<int>( AblScore::Wis ), false ); }
void NwnCharGen::on_pushButtonWisPlus_clicked() { modAbility( static_cast<int>( AblScore::Wis ), true ); }
void NwnCharGen::on_pushButtonChaMinus_clicked() { modAbility( static_cast<int>( AblScore::Cha ), false ); }
void NwnCharGen::on_pushButtonChaPlus_clicked() { modAbility( static_cast<int>( AblScore::Cha ), true ); }

void NwnCharGen::on_buttonRace_clicked()
{
    RaceDialog rd(nwnRules, this);
    if( rd.exec() == QDialog::Accepted ) {
        if( !rd.raceChoice.isEmpty() ) {
            nwnChar->setRace( rd.raceChoice.toStdString() );
            setDirtyFlag();
            updateSummary();
        }
    }
}

void NwnCharGen::on_actionSave_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save Character"),
                                                    currentFile,
                                                    tr("Character files (*.xml)"));
    if( !fileName.isNull() ) {
        nwnChar->setDescription( ui->textEditDescription->toPlainText().toStdString() );
        nwnChar->save( qPrintable( fileName ) );
        currentFile = fileName;
        clearDirtyFlag();
    }
}

void NwnCharGen::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Character"),
                                                    currentFile,
                                                    tr("Character files (*.xml)"));
    if( !fileName.isNull() ) {
        nwnChar->restore( qPrintable( fileName ) );
        currentFile = fileName;
        clearDirtyFlag();
        updateAll();
    }
}

void NwnCharGen::on_comboBoxAlignment_currentIndexChanged(int index)
{
    nwnChar->setAlignment( static_cast<Alignment>( index ) );
    setDirtyFlag();
    updateAll();
}

void NwnCharGen::on_actionAbout_triggered()
{
    QMessageBox::about( this, "NwnCharGen", "A character builder for Neverwinter Nights 2" );
}

void NwnCharGen::on_actionExit_triggered()
{
    qApp->exit();
}

void NwnCharGen::on_actionNew_triggered()
{
    nwnChar = std::make_unique<Character>();
    updateAll();
    clearDirtyFlag();
}

void NwnCharGen::on_lineEditName_editingFinished()
{
    nwnChar->setName( ui->lineEditName->text().toStdString() );
    setDirtyFlag();
}

void NwnCharGen::updateWindowTitle()
{
    QString title = dirtyFlag ? "*" : "";
    title += currentFile;
    title += " [" + currentRules + "]";
    title += " - NwnCharGen";
    this->setWindowTitle( title );
}

void NwnCharGen::setDirtyFlag()
{
    dirtyFlag = true;
    updateWindowTitle();
}

void NwnCharGen::clearDirtyFlag()
{
    dirtyFlag = false;
    updateWindowTitle();
}

void NwnCharGen::on_textEditDescription_textChanged()
{
    setDirtyFlag();
}

void NwnCharGen::on_actionRulesSave_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save Rules"),
                                                    currentRules,
                                                    tr("Rules files (*.xml)"));
    if( !fileName.isNull() ) {
        nwnRules->save( qPrintable( fileName ) );
        currentRules = fileName;
//        clearDirtyFlag();
    }
}
