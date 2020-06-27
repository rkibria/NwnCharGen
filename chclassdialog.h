#ifndef CHCLASSDIALOG_H
#define CHCLASSDIALOG_H

#include <QDialog>

namespace Ui {
class ChClassDialog;
}

namespace Nwn {
class Rules;
class Character;
}

class ChClassDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChClassDialog( Nwn::Rules *rules, Nwn::Character *nwnchar, QWidget *parent = nullptr );
    ~ChClassDialog();

    const QString& getChoice() const { return chClassChoice; }
    bool haveChoice() const { return !chClassChoice.isEmpty(); }

private slots:
    void on_listWidgetChClasses_itemSelectionChanged();

private:
    Ui::ChClassDialog *ui;
    Nwn::Rules *nwnRules;
    Nwn::Character *nwnChar;
    QString chClassChoice;
    void initChClassesWidget();
    void updateOkButton();
};

#endif // CHCLASSDIALOG_H
