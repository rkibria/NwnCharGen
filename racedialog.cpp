#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>

#include "racedialog.h"
#include "ui_racedialog.h"

#include <Nwn/race.hpp>
#include <Nwn/rules.hpp>

#include <unordered_map>

using namespace Nwn;

RaceDialog::RaceDialog( Nwn::Rules *rules, QWidget *parent ) :
    QDialog( parent ),
    ui( new Ui::RaceDialog ),
    nwnRules{ rules },
    raceChoice()
{
    ui->setupUi( this );

    setWidgetsChoiceOnly();
    setupRacesWidget();
    updateOkButton();
}

RaceDialog::~RaceDialog()
{
    delete ui;
}

void RaceDialog::setupRacesWidget()
{
    ui->treeWidgetRace->clear();
    ui->treeWidgetRace->setColumnCount( 1 );
    ui->treeWidgetRace->setHeaderHidden( true );

    ui->treeWidgetRace->setSortingEnabled( false );
    std::unordered_map<std::string, QTreeWidgetItem*> classItems;
    for( const auto& race : nwnRules->getRaces() ) {
        const auto& classification = race.getClassification();

        QTreeWidgetItem* classItem;
        if( classItems.find( classification ) != classItems.end() ) {
            classItem = classItems[ classification ];
        }
        else {
            classItem = new QTreeWidgetItem( ui->treeWidgetRace, QStringList( QString( classification.c_str() ) ) );
            classItem->setFlags( classItem->flags() & ~Qt::ItemIsSelectable );
            classItems[ classification ] = classItem;
        }

        new QTreeWidgetItem( classItem, QStringList( QString( race.getName().c_str() ) ) );
    }

    ui->treeWidgetRace->setSortingEnabled( true );
    ui->treeWidgetRace->sortByColumn( 0, Qt::AscendingOrder );

    ui->treeWidgetRace->expandAll();
}

void RaceDialog::setWidgetsChoiceOnly()
{
    setWindowTitle( tr( "Choose Race" ) );

    QLayoutItem *child;
    while( ( child = ui->horizontalLayoutButtons->takeAt(0) ) != 0 ) {
        delete child;
    }
}

void RaceDialog::on_treeWidgetRace_itemSelectionChanged()
{
    const auto selection = ui->treeWidgetRace->selectedItems();
    if( selection.size() ) {
        const auto item = selection.first();

        raceChoice = item->text( 0 );

        const auto& description = nwnRules->getRaceByName( raceChoice.toStdString() )->getDescription();
        ui->textEditDescription->setHtml( QString::fromStdString( description ) );

        updateOkButton();
    }
}

void RaceDialog::updateOkButton()
{
    ui->buttonBox->button( QDialogButtonBox::Ok )->setEnabled( haveChoice() );
}
