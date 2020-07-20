#include <QPushButton>

#include "chclassdialog.h"
#include "ui_chclassdialog.h"

#include <Nwn/rules.hpp>
#include <Nwn/chclass.hpp>

ChClassDialog::ChClassDialog( Nwn::Rules *rules, Nwn::Character *nwnchar, QWidget *parent ) :
    QDialog(parent),
    ui(new Ui::ChClassDialog),
    nwnRules( rules ),
    nwnChar( nwnchar ),
    chClassChoice()
{
    ui->setupUi(this);

    initChClassesWidget();
    updateOkButton();
}

ChClassDialog::~ChClassDialog()
{
    delete ui;
}

void ChClassDialog::initChClassesWidget()
{
    for( const auto& chclass : nwnRules->getChClasses() ) {
        new QListWidgetItem( chclass.getName().c_str(), ui->listWidgetChClasses );
    }
    ui->listWidgetChClasses->setSortingEnabled( true );
    ui->listWidgetChClasses->sortItems();
}

void ChClassDialog::updateOkButton()
{
    ui->buttonBox->button( QDialogButtonBox::Ok )->setEnabled( haveChoice() );
}

void ChClassDialog::on_listWidgetChClasses_itemSelectionChanged()
{
    const auto selection = ui->listWidgetChClasses->selectedItems();
    if( selection.size() ) {
        const auto item = selection.first();

        chClassChoice = item->text();

        const auto& description = nwnRules->getChClassByName( chClassChoice.toStdString() ).getDescription();
        ui->textEditChClassDescription->setHtml( QString::fromStdString( description ) );

        updateOkButton();
    }
}
