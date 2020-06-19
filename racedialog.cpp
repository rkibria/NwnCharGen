#include "racedialog.h"
#include "ui_racedialog.h"

#include <nwnrace.hpp>
#include <nwnrules.hpp>

#include <unordered_map>

using namespace Nwn;

RaceDialog::RaceDialog( const std::unique_ptr<Rules>& rules, QWidget *parent ) :
    QDialog( parent ),
    ui( new Ui::RaceDialog ),
    nwnRules{ rules }
{
    ui->setupUi( this );

    ui->treeWidgetRace->setColumnCount( 1 );
    ui->treeWidgetRace->setHeaderHidden( true );

    QList<QTreeWidgetItem *> items;

    std::unordered_map<std::string, QTreeWidgetItem*> classItems;
    for( const auto& race : rules->getRaces() ) {
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

RaceDialog::~RaceDialog()
{
    delete ui;
}

void RaceDialog::on_treeWidgetRace_itemSelectionChanged()
{
    const auto selection = ui->treeWidgetRace->selectedItems();
    if( selection.size() ) {
        const auto item = selection.first();
        raceChoice = item->text( 0 );
        const auto& description = nwnRules->getRaceByName( raceChoice.toStdString() ).getDescription();
        ui->textEditDescription->setText( QString::fromStdString( description ) );
    }
}
