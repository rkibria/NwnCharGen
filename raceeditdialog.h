#ifndef RACEEDITDIALOG_H
#define RACEEDITDIALOG_H

#include <QDialog>

#include <memory>

namespace Ui {
class RaceEditDialog;
}

namespace Nwn {
class Rules;
class Race;
}

class RaceEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RaceEditDialog( Nwn::Rules *rules, const std::string& name, QWidget *parent = nullptr );
    ~RaceEditDialog();

private:
    Ui::RaceEditDialog *ui;
    Nwn::Rules *nwnRules;
    const std::string raceName;
    std::unique_ptr<Nwn::Race> raceCopy;
    void updateAll();
};

#endif // RACEEDITDIALOG_H
