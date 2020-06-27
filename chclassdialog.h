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

private:
    Ui::ChClassDialog *ui;
    Nwn::Rules *nwnRules;
    Nwn::Character *nwnChar;
};

#endif // CHCLASSDIALOG_H
