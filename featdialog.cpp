#include <QInputDialog>
#include <QMessageBox>

#include "featdialog.h"
#include "ui_featdialog.h"

#include <Nwn/feat.hpp>
#include <Nwn/rules.hpp>

#include <unordered_map>

using namespace Nwn;

FeatDialog::FeatDialog(Nwn::Rules *rules, std::unique_ptr< std::set< int > > bonusChoices, int lvl, Nwn::Character* nwnchar, QWidget *parent ) :
    QDialog( parent ),
    ui( new Ui::FeatDialog ),
    nwnRules{ rules },
    bonusChoices{ std::move( bonusChoices ) },
    lvl{ lvl },
    nwnChar{ nwnchar }
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
    ui->treeWidgetFeat->clear();
    ui->treeWidgetFeat->setColumnCount( 1 );
    ui->treeWidgetFeat->setHeaderHidden( true );

    ui->treeWidgetFeat->setSortingEnabled( false );
    for( const auto& feat : nwnRules->getFeats() ) {
        if( bonusChoices && bonusChoices->find( feat.getId() ) == bonusChoices->end() ) {
            continue;
        }

        if( !nwnRules->isFeatAvailAtLvl( nwnChar, lvl, feat.getId() ) ) {
            continue;
        }

        new QTreeWidgetItem( ui->treeWidgetFeat,
                             QStringList( QString( feat.getName().c_str() ) ),
                             feat.getId() );
    }

    ui->treeWidgetFeat->setSortingEnabled( true );
    ui->treeWidgetFeat->sortByColumn( 0, Qt::AscendingOrder );

    ui->treeWidgetFeat->expandAll();
}

void FeatDialog::on_treeWidgetFeat_itemSelectionChanged()
{
    const auto selection = ui->treeWidgetFeat->selectedItems();
    if( selection.size() ) {
        const auto item = selection.first();
        const auto featId = item->type();

        featChoice = featId;

        const auto& description = nwnRules->getFeat( featId )->getDescription();
        ui->textEditDescription->setHtml( QString::fromStdString( description ) );
    }

}
