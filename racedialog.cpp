#include <QInputDialog>

#include "racedialog.h"
#include "ui_racedialog.h"

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

    ui->treeWidgetRace->expandAll();
}

void RaceDialog::setWidgetsChoiceOnly()
{
    if( isChoiceOnly ) {
        setWindowTitle( tr( "Choose Race" ) );
        QLayoutItem *child;
        while ((child = ui->horizontalLayoutButtons->takeAt(0)) != 0) {
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

        const auto& description = nwnRules->getRaceByName( raceChoice.toStdString() ).getDescription();
        ui->textEditDescription->setText( QString::fromStdString( description ) );

        updateEditButtons();
        updateOkButton();
    }
}

void RaceDialog::on_pushButtonNew_clicked()
{
    bool ok;
    QString raceName = QInputDialog::getText( this, "New Race", "Name of the race:", QLineEdit::Normal, "", &ok );
    if ( ok && !raceName.isEmpty() ) {
        nwnRules->setRace( std::make_unique<Race>( raceName.toStdString(), "Unknown" ) );
        setupRacesWidget();
    }
}

void RaceDialog::on_pushButtonEdit_clicked()
{

}

void RaceDialog::on_pushButtonDelete_clicked()
{

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
