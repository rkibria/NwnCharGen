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
    explicit RaceDialog(const std::unique_ptr<Nwn::Rules>& rules, QWidget *parent = nullptr);
    ~RaceDialog();

    QString raceChoice;

private slots:
    void on_treeWidgetRace_itemSelectionChanged();

private:
    Ui::RaceDialog *ui;
    const std::unique_ptr<Nwn::Rules>& nwnRules;
};

#endif // RACEDIALOG_H
