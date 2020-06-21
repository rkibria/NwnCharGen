#ifndef RULESRACESDIALOG_H
#define RULESRACESDIALOG_H

#include <QDialog>

namespace Ui {
class RulesRacesDialog;
}

namespace Nwn {
class Rules;
}

class RulesRacesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RulesRacesDialog( const Nwn::Rules *rules, QWidget *parent = nullptr );
    ~RulesRacesDialog();

private:
    Ui::RulesRacesDialog *ui;
    const Nwn::Rules *nwnRules;
};

#endif // RULESRACESDIALOG_H
