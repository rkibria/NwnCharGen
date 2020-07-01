#include "levelsmodel.h"
#include "nwnchargen.h"

#include "Nwn/character.hpp"
using namespace Nwn;

constexpr const int LevelsModel::kLevelCol;
constexpr const int LevelsModel::kClassCol;
constexpr const int LevelsModel::kHpCol;
constexpr const int LevelsModel::kBabCol;
constexpr const int LevelsModel::kSTRCol;
constexpr const int LevelsModel::kDEXCol;
constexpr const int LevelsModel::kCONCol;
constexpr const int LevelsModel::kINTCol;
constexpr const int LevelsModel::kWISCol;
constexpr const int LevelsModel::kCHACol;

constexpr const int LevelsModel::kColCount;

constexpr const std::array<int, 6> LevelsModel::ablCols;

LevelsModel::LevelsModel( QObject *parent )
    : QAbstractTableModel( parent ),
      nwnCharGen{ static_cast<NwnCharGen*>( parent ) }
{
}

QVariant LevelsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if( role == Qt::DisplayRole && orientation == Qt::Horizontal ) {
        switch( section ) {
        case kLevelCol: return QString("Lvl");
        case kClassCol: return QString("Class");
        case kHpCol:    return QString("HP");
        case kBabCol:   return QString("BAB");
        case kSTRCol:   return QString("STR");
        case kDEXCol:   return QString("DEX");
        case kCONCol:   return QString("CON");
        case kINTCol:   return QString("INT");
        case kWISCol:   return QString("WIS");
        case kCHACol:   return QString("CHA");
        default: return QVariant();
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

    return kColCount;
}

QVariant LevelsModel::data(const QModelIndex &index, int role) const
{
    if( index.column() == kLevelCol && role == Qt::TextAlignmentRole) {
        return Qt::AlignCenter;
    }

    if( !index.isValid() || role != Qt::DisplayRole )
        return QVariant();

    if( index.row() >= nwnCharGen->getCharacter()->getNumLevels() ) {
        return QVariant();
    }

    if( index.column() == kLevelCol ) {
        return index.row() + 1;
    }
    else if( index.column() == kClassCol ) {
        return QVariant( nwnCharGen->getCharacter()->getLevel( index.row() ).c_str() );
    }

    return QVariant();
}

void LevelsModel::updateView()
{
    emit layoutAboutToBeChanged();
    emit layoutChanged();
}
