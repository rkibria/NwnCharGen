#include <QInputDialog>
#include <QMessageBox>

#include "racedialog.h"
#include "ui_racedialog.h"

#include "raceeditdialog.h"

#include <Nwn/race.hpp>
#include <Nwn/rules.hpp>

#include <unordered_map>

using namespace Nwn;

RaceDialog::RaceDialog( Nwn::Rules *rules, bool choiceOnly, QWidget *parent ) :
    QDialog( parent ),
    ui( new Ui::RaceDialog ),
    nwnRules{ rules },
    raceChoice(),
    isChoiceOnly{ choiceOnly }
{
    ui->setupUi( this );

    setWidgetsChoiceOnly();
    setupRacesWidget();
    updateEditButtons();
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
    if( isChoiceOnly ) {
        setWindowTitle( tr( "Choose Race" ) );

        ui->pushButtonNew->setVisible( false );
        ui->pushButtonEdit->setVisible( false );
        ui->pushButtonDelete->setVisible( false );

        QLayoutItem *child;
        while( ( child = ui->horizontalLayoutButtons->takeAt(0) ) != 0 ) {
            delete child;
        }
    }
    else {
        ui->buttonBox->setStandardButtons( QDialogButtonBox::Ok );
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

        updateEditButtons();
        updateOkButton();
    }
}

void RaceDialog::on_pushButtonNew_clicked()
{
    bool ok;
    QString raceName = QInputDialog::getText( this, "New Race", "Name of the race:\n(initial classification will be NEW)", QLineEdit::Normal, "", &ok );
    if ( ok ) {
        if( !raceName.isEmpty() ) {
            nwnRules->setRace( std::make_unique<Race>( raceName.toStdString(), "NEW" ) );
            setupRacesWidget();
        }
        else {
            QMessageBox::critical( this, "Error", "Name must not be empty!" );
        }
    }
}

void RaceDialog::on_pushButtonEdit_clicked()
{
    RaceEditDialog rd( nwnRules, getChoice().toStdString(), this );
    if( rd.exec() == QDialog::Accepted ) {
        nwnRules->removeRace( rd.getOriginalName() );
        nwnRules->setRace( std::move( rd.getEdited() ) );
        setupRacesWidget();
    }
}

void RaceDialog::on_pushButtonDelete_clicked()
{
    const auto button = QMessageBox::question( this, "Delete", QString( "%1 will be removed. Continue?" ).arg( getChoice() ) );
    if( button == QMessageBox::Yes ) {
        nwnRules->removeRace( getChoice().toStdString() );
        setupRacesWidget();
    }
}

void RaceDialog::updateEditButtons()
{
    if( !isChoiceOnly ) {
        ui->pushButtonEdit->setEnabled( haveChoice() );
        ui->pushButtonDelete->setEnabled( haveChoice() );
    }
}

void RaceDialog::updateOkButton()
{
    if( isChoiceOnly ) {
        ui->buttonBox->button( QDialogButtonBox::Ok )->setEnabled( haveChoice() );
    }
}
