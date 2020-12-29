#ifndef FEATDIALOG_H
#define FEATDIALOG_H

#include <QDialog>
#include <memory>

namespace Ui {
class FeatDialog;
}

namespace Nwn {
class Rules;
}

class FeatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FeatDialog( Nwn::Rules *rules, bool choiceOnly, QWidget *parent );
    ~FeatDialog();

    int getFeatChoice() const { return featChoice; }

private slots:
    void on_treeWidgetFeat_itemSelectionChanged();

private:
    Ui::FeatDialog *ui;
    Nwn::Rules *nwnRules;
    int featChoice = -1;

    void setupWidget();
};

#endif // FEATDIALOG_H
