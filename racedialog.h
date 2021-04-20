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
    explicit RaceDialog( Nwn::Rules *rules, QWidget *parent );
    ~RaceDialog();

    const QString& getChoice() const { return raceChoice; }

private slots:
    void on_treeWidgetRace_itemSelectionChanged();

private:
    Ui::RaceDialog *ui;
    Nwn::Rules *nwnRules;
    QString raceChoice;

    bool haveChoice() const { return !raceChoice.isEmpty(); }

    void setWidgetsChoiceOnly();
    void setupRacesWidget();
    void updateOkButton();
};

#endif // RACEDIALOG_H
