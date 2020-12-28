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

private:
    Ui::FeatDialog *ui;
    Nwn::Rules *nwnRules;

    void setupWidget();
};

#endif // FEATDIALOG_H
