#ifndef LEVELSMODEL_H
#define LEVELSMODEL_H

#include <QAbstractTableModel>

class NwnCharGen;

class LevelsModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit LevelsModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    NwnCharGen* nwnCharGen;
};

#endif // LEVELSMODEL_H
