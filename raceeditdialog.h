#ifndef RACEEDITDIALOG_H
#define RACEEDITDIALOG_H

#include <QDialog>

#include <memory>
#include <string>

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

    const std::string& getOriginalName() const { return raceName; }
    std::unique_ptr<Nwn::Race>& getEdited() { return raceCopy; }

public slots:
    void done( int r ) override;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::RaceEditDialog *ui;
    Nwn::Rules *nwnRules;
    const std::string raceName;
    std::unique_ptr<Nwn::Race> raceCopy;

    void updateAll();
};

#endif // RACEEDITDIALOG_H
