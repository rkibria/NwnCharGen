#include "levelsmodel.h"
#include "nwnchargen.h"

#include "Nwn/character.hpp"
using namespace Nwn;

LevelsModel::LevelsModel( QObject *parent )
    : QAbstractTableModel( parent ),
      nwnCharGen{ static_cast<NwnCharGen*>( parent ) }
{
}

QVariant LevelsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        if (section == 0) {
            return QString("Level");
        } else if (section == 1) {
            return QString("Class");
        }
    }
    return QVariant();
}

int LevelsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return nwnCharGen->getCharacter()->getNumLevels();
}

int LevelsModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 2;
}

QVariant LevelsModel::data(const QModelIndex &index, int role) const
{
    if( !index.isValid() || role != Qt::DisplayRole )
        return QVariant();

    if( index.row() >= nwnCharGen->getCharacter()->getNumLevels() ) {
        return QVariant();
    }

    if( index.column() == 0 ) {
        return index.row() + 1;
    }
    else if( index.column() == 1 ) {
        return QVariant( nwnCharGen->getCharacter()->getLevel( index.row() ).c_str() );
    }

    return QVariant();
}

void LevelsModel::updateView()
{
    emit layoutAboutToBeChanged();
    emit layoutChanged();
}
