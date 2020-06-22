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
    explicit RaceDialog( Nwn::Rules *rules, bool choiceOnly, QWidget *parent );
    ~RaceDialog();

    const QString& getChoice() const { return raceChoice; }

private slots:
    void on_treeWidgetRace_itemSelectionChanged();
    void on_pushButtonNew_clicked();
    void on_pushButtonEdit_clicked();
    void on_pushButtonDelete_clicked();

private:
    Ui::RaceDialog *ui;
    Nwn::Rules *nwnRules;
    QString raceChoice;
    const bool isChoiceOnly;

    void setWidgetsChoiceOnly();
    void setupRacesWidget();
};

#endif // RACEDIALOG_H
