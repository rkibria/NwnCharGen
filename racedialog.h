#ifndef RACEDIALOG_H
#define RACEDIALOG_H

#include <QDialog>
#include <memory>

namespace Ui {
class RaceDialog;
}

namespace Nwn {
class Rules;
}

class RaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RaceDialog(const Nwn::Rules *rules, bool choiceOnly, QWidget *parent);
    ~RaceDialog();

    QString raceChoice;

private slots:
    void on_treeWidgetRace_itemSelectionChanged();

private:
    Ui::RaceDialog *ui;
    const Nwn::Rules *nwnRules;

    void setWidgetsChoiceOnly( bool choiceOnly );
    void setupRacesWidget();
};

#endif // RACEDIALOG_H
