#ifndef NWNCHARGEN_H
#define NWNCHARGEN_H

#include <QMainWindow>
#include <memory>
#include <array>

QT_BEGIN_NAMESPACE
namespace Ui { class NwnCharGen; }
QT_END_NAMESPACE

class QLineEdit;

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

    Nwn::Character* getCharacter() { return nwnChar.get(); }
    Nwn::Rules* getRules() { return nwnRules.get(); }

    void updateAll();

public slots:
    void customMenuRequested( const QPoint &pos );
    void menuOverwriteFollowing();

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
    void on_textEditDescription_textChanged();
    void on_actionRulesSave_triggered();
    void on_actionRulesOpen_triggered();
    void on_actionRaces_triggered();
    void on_spinBoxLevels_valueChanged(int arg1);

private:
    Ui::NwnCharGen *ui;

    std::unique_ptr<Nwn::Character> nwnChar;
    std::unique_ptr<Nwn::Rules> nwnRules;

    bool dirtyFlag;
    QString currentFile;
    QString currentRules;

    void updateAbilityBlock();
    void updateSummary();
    void updateWindowTitle();

    void setDirtyFlag();
    void clearDirtyFlag();

    void modAbility( int abl, bool isInc );
    void initWidgets();
    void updateLevels();
    void updateClasses();
    void initLevelsWidget();

    std::array< QLineEdit*, 6 > getAblLineEdits() const;
    std::array< QLineEdit*, 6 > getAblModLineEdits() const;
    std::array< QLineEdit*, 6 > getFinalAblLineEdits() const;
    std::array< QLineEdit*, 6 > getFinalAblModLineEdits() const;
};

#endif // NWNCHARGEN_H
