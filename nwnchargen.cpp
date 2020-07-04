#include <QFileDialog>
#include <QMessageBox>
#include <QMap>

#include "nwnchargen.h"
#include "./ui_nwnchargen.h"

#include <Nwn/base.hpp>
#include <Nwn/character.hpp>
#include <Nwn/ablblock.hpp>
#include <Nwn/race.hpp>
#include <Nwn/rules.hpp>
#include <Nwn/chclass.hpp>

#include "racedialog.h"
#include "levelsmodel.h"
#include "levelsclassdelegate.h"
#include "levelsabldelegate.h"

using namespace Nwn;

namespace {

static constexpr const char* kDefaultClass = "Barbarian";

void addDebugRules( Rules* nwnRules )
{
    std::unique_ptr<Race> human = std::make_unique<Race>( "Human", "Human" );
    human->setDescription("Humans are the most adaptable of the common races. Short generations and a penchant for migration and conquest mean they are very physically diverse as well. Skin shades range from nearly black to very pale, hair from black to blond, and facial hair (for men) from sparse to thick. Humans are often unorthodox in their dress, sporting unusual hairstyles, fanciful clothes, tattoos, and the like.");
    nwnRules->setRace( std::move( human ) );

    std::unique_ptr<Race> goldDwarf = std::make_unique<Race>( "Gold Dwarf", "Dwarf" );
    goldDwarf->setDescription("The gold dwarves maintained their great kingdom in the Great Rift and managed to survive terrible wars against evil humanoids. While they practiced some magic, they never acquired the hubris that caused the downfall of some human nations. Confident and secure in their remote home, the gold dwarves gained a reputation for haughtiness and pride. Since the Thunder Blessing, many young gold dwarves have left the Great Rift and are exploring the rest of the region. The folk of other lands have learned that while some gold dwarves are aloof and suspicious, for the most part they are forthright warriors and shrewd traders.");
    goldDwarf->getAblAdjusts().setAbl( AblScore::Con, 2 );
    goldDwarf->getAblAdjusts().setAbl( AblScore::Dex, -2 );
    nwnRules->setRace( std::move( goldDwarf ) );

    nwnRules->setRace( std::make_unique<Race>( "Gray Dwarf", "Dwarf" ) );
    nwnRules->setRace( std::make_unique<Race>( "Drow", "Elf" ) );

    std::unique_ptr< ChClass > barbarian = std::make_unique< ChClass >( "Barbarian" );
    barbarian->setDescription( "Barbarians are brave, even reckless warriors whose great strength and heartiness makes them well suited for adventure. Where the fighter would rely on training and discipline, the barbarian enters a berserker state that makes him stronger, tougher, and more determined but less concerned with his health. These spectacular rages leave him winded, and he only has the energy for a few a day, but those usually suffice. He also knows the wild and runs at great speed." );
    nwnRules->setChClass( std::move( barbarian ) );

    std::unique_ptr< ChClass > cleric = std::make_unique< ChClass >( "Cleric" );
    cleric->setDescription( "Clerics are masters of divine magic, which is especially good at healing. Even an inexperienced Cleric can bring people back from the brink of death, and an experienced Cleric can bring back people who have crossed over that brink. As channelers of divine energy, clerics can affect undead creatures. A Cleric can turn away or even destroy undead. Clerics have some combat training. They can use simple weapons, and they are trained in the use of armor, since armor does not interfere with divine spells the way it does with arcane spells." );
    nwnRules->setChClass( std::move( cleric ) );
}

void initCharacter( Character* nwnChar )
{
    for( int i = 0; i < 30; ++i ) {
        nwnChar->pushLevel( kDefaultClass );
    }
}

} // namespace

NwnCharGen::NwnCharGen(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::NwnCharGen)
    , nwnChar( std::make_unique<Character>() )
    , nwnRules( std::make_unique<Rules>() )
    , dirtyFlag{ false }
    , currentFile{ "character.xml" }
    , currentRules()
{
    // TODO load from disk on startup
    currentRules = "nwn2.xml";
    addDebugRules( nwnRules.get() );

    initCharacter( nwnChar.get() );

    ui->setupUi(this);
    initWidgets();

    updateAll();
    clearDirtyFlag();
}

NwnCharGen::~NwnCharGen()
{
    delete ui;
}

void NwnCharGen::initLevelsWidget()
{
    const auto levelsModel = new LevelsModel( this );
    ui->tableViewLevels->setModel( levelsModel );
    ui->tableViewLevels->setItemDelegateForColumn( LevelsModel::kClassCol, new LevelsClassDelegate( ui->tableViewLevels ) );

    ui->tableViewLevels->setItemDelegateForColumn( LevelsModel::kSTRCol, new LevelsAblDelegate( ui->tableViewLevels ) );

    const auto fm = this->fontMetrics();
    ui->tableViewLevels->setColumnWidth( LevelsModel::kLevelCol, fm.horizontalAdvance( "30" ) );
    ui->tableViewLevels->setColumnWidth( LevelsModel::kHpCol, fm.horizontalAdvance( "999" ) );
    ui->tableViewLevels->setColumnWidth( LevelsModel::kBabCol, fm.horizontalAdvance( "+99" ) );

    const auto ablColWidth = fm.horizontalAdvance( "99" );
    for( const auto col : LevelsModel::ablCols ) {
        ui->tableViewLevels->setColumnWidth( col, ablColWidth );
    }

    const auto hh = ui->tableViewLevels->horizontalHeader();
    hh->setSectionResizeMode( LevelsModel::kLevelCol, QHeaderView::Fixed );
    hh->setSectionResizeMode( LevelsModel::kHpCol, QHeaderView::Fixed );
    hh->setSectionResizeMode( LevelsModel::kBabCol, QHeaderView::Fixed );

    for( const auto col : LevelsModel::ablCols ) {
        hh->setSectionResizeMode( col, QHeaderView::Fixed );
    }
}

void NwnCharGen::initWidgets()
{
    setCentralWidget(ui->tabWidget);

    for( const auto& aln : allAlignments ) {
        ui->comboBoxAlignment->addItem( alignmentToStr( aln ).c_str() );
    }

    initLevelsWidget();
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

void NwnCharGen::updateClasses()
{
    QMap<QString, int> curClassLvls;
    for( int i = 0; i < nwnChar->getNumLevels(); ++i ) {
        curClassLvls[ nwnChar->getLevel( i ).c_str() ]++;
    }
    QString output;
    auto itr = curClassLvls.constBegin();
    while( itr != curClassLvls.constEnd() ) {
        const auto curCls = QString( "%1 (%2) " ).arg( itr.key() ).arg( itr.value() );
        output += curCls;
        ++itr;
    }
    ui->lineEditClasses->setText( output );
}

void NwnCharGen::updateSummary()
{
    ui->lineEditName->setText( nwnChar->getName().c_str() );
    ui->textEditDescription->setText( nwnChar->getDescription().c_str() );
    ui->lineEditRace->setText( nwnChar->getRace().c_str() );
    ui->comboBoxAlignment->setCurrentIndex( alignmentToIndex( nwnChar->getAlignment() ) );
    ui->spinBoxLevels->setValue( nwnChar->getNumLevels() );

    updateClasses();

    updateAbilityBlock();
}

void NwnCharGen::updateLevels()
{
    if( auto levelsModel = static_cast<LevelsModel*>( ui->tableViewLevels->model() ) ) {
        levelsModel->updateView();
    }
}

void NwnCharGen::updateAll()
{
    updateSummary();
    updateLevels();
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
    RaceDialog rd( nwnRules.get(), true, this );
    if( rd.exec() == QDialog::Accepted ) {
        if( !rd.getChoice().isEmpty() ) {
            nwnChar->setRace( rd.getChoice().toStdString() );
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

void NwnCharGen::on_comboBoxAlignment_currentIndexChanged( int index )
{
    nwnChar->setAlignment( indexToAlignment( index ) );
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
    }
}

void NwnCharGen::on_actionRulesOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Rules"),
                                                    currentRules,
                                                    tr("Rules files (*.xml)"));
    if( !fileName.isNull() ) {
        nwnRules->restore( qPrintable( fileName ) );
        currentRules = fileName;
        updateAll();
    }
}

void NwnCharGen::on_actionRaces_triggered()
{
    RaceDialog rd( nwnRules.get(), false, this );
    rd.exec();
    updateAll();
}

void NwnCharGen::on_spinBoxLevels_valueChanged( int newLvls )
{
    const auto curLvls = nwnChar->getNumLevels();
    if( newLvls > curLvls ) {
        for( int i = 0; i < newLvls - curLvls; ++i ) {
            nwnChar->pushLevel( kDefaultClass );
        }
        setDirtyFlag();
        updateAll();
    }
    else if( newLvls < curLvls ) {
        if( QMessageBox::warning( this,
                                  "Remove levels",
                                  "Really remove levels? Any settings will be lost.",
                                  QMessageBox::Ok | QMessageBox::Cancel,
                                  QMessageBox::Cancel ) == QMessageBox::Ok ) {
            for( int i = 0; i < curLvls - newLvls; ++i ) {
                nwnChar->popLevel();
            }
            setDirtyFlag();
            updateAll();
        }
        else {
            ui->spinBoxLevels->setValue( curLvls );
        }
    }
}
