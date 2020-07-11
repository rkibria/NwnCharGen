#ifndef LEVELSMODEL_H
#define LEVELSMODEL_H

#include <array>

#include <QAbstractTableModel>

#include <Nwn/base.hpp>

class NwnCharGen;

class LevelsModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum LevelColumns {
        kLevelCol,
        kClassCol,
        kHpCol,
        kBabCol,
        kSTRCol,
        kDEXCol,
        kCONCol,
        kINTCol,
        kWISCol,
        kCHACol,

        kColCount
    };

    static constexpr const std::array<int, 6> ablCols = {
        LevelColumns::kSTRCol,
        LevelColumns::kDEXCol,
        LevelColumns::kCONCol,
        LevelColumns::kINTCol,
        LevelColumns::kWISCol,
        LevelColumns::kCHACol
    };

    static Nwn::AblScore getColumnAbl( int col );

public:
    explicit LevelsModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void updateView();
    NwnCharGen* getNwnCharGen() const { return nwnCharGen; }

private:
    NwnCharGen* nwnCharGen;
};

#endif // LEVELSMODEL_H
