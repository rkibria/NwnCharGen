#include <QInputDialog>
#include <QMessageBox>

#include "featdialog.h"
#include "ui_featdialog.h"

#include <Nwn/feat.hpp>
#include <Nwn/rules.hpp>

#include <unordered_map>

using namespace Nwn;

FeatDialog::FeatDialog( Nwn::Rules *rules, bool choiceOnly, QWidget *parent ) :
    QDialog( parent ),
    ui( new Ui::FeatDialog ),
    nwnRules{ rules }
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
        if( !feat.getAllClassesCanUse() ) {
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
