#ifndef NWNCHARGEN_H
#define NWNCHARGEN_H

#include <QMainWindow>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class NwnCharGen; }
QT_END_NAMESPACE

namespace Nwn {
    class Character;
    class Rules;
}

class NwnCharGen : public QMainWindow
{
    Q_OBJECT

public:
    NwnCharGen(QWidget *parent = nullptr);
    ~NwnCharGen();

private slots:
    void on_pushButtonStrMinus_clicked();
    void on_pushButtonStrPlus_clicked();
    void on_pushButtonDexMinus_clicked();
    void on_pushButtonDexPlus_clicked();
    void on_pushButtonConMinus_clicked();
    void on_pushButtonConPlus_clicked();
    void on_pushButtonIntMinus_clicked();
    void on_pushButtonIntPlus_clicked();
    void on_pushButtonWisMinus_clicked();
    void on_pushButtonWisPlus_clicked();
    void on_pushButtonChaMinus_clicked();
    void on_pushButtonChaPlus_clicked();

    void on_buttonRace_clicked();
    void on_actionSave_triggered();
    void on_actionOpen_triggered();
    void on_comboBoxAlignment_currentIndexChanged(int index);
    void on_actionAbout_triggered();
    void on_actionExit_triggered();
    void on_actionNew_triggered();
    void on_lineEditName_editingFinished();

private:
    Ui::NwnCharGen *ui;

    std::unique_ptr<Nwn::Character> nwnChar;
    std::unique_ptr<Nwn::Rules> nwnRules;

    bool dirtyFlag;
    QString currentFile;

    void updateAbilityBlock();
    void updateSummary();
    void updateAll();
    void updateWindowTitle();

    void setDirtyFlag();
    void clearDirtyFlag();
};

#endif // NWNCHARGEN_H
