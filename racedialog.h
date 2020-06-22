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

    QString raceChoice;

private slots:
    void on_treeWidgetRace_itemSelectionChanged();
    void on_pushButtonNew_clicked();
    void on_pushButtonEdit_clicked();
    void on_pushButtonDelete_clicked();

private:
    Ui::RaceDialog *ui;
    Nwn::Rules *nwnRules;

    void setWidgetsChoiceOnly( bool choiceOnly );
    void setupRacesWidget();
};

#endif // RACEDIALOG_H
