#ifndef FEATDIALOG_H
#define FEATDIALOG_H

#include <QDialog>
#include <memory>
#include <set>

namespace Ui {
class FeatDialog;
}

namespace Nwn {
class Rules;
class Character;
}

class FeatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FeatDialog( Nwn::Rules *rules, const std::set< int >* bonusChoices, int lvl, Nwn::Character* nwnchar, QWidget *parent );
    ~FeatDialog();

    int getFeatChoice() const { return featChoice; }

private slots:
    void on_treeWidgetFeat_itemSelectionChanged();

private:
    Ui::FeatDialog *ui;
    Nwn::Rules *nwnRules;
    int featChoice = -1;
    const std::set< int > *pBonusChoices;
    const int lvl;
    Nwn::Character *nwnChar;

    void setupWidget();
};

#endif // FEATDIALOG_H
