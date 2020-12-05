#include <stdexcept>

#include "levelsmodel.h"
#include "nwnchargen.h"

#include "Nwn/character.hpp"
#include "Nwn/ablblock.hpp"
#include "Nwn/rules.hpp"

using namespace Nwn;

constexpr const std::array<int, 6> LevelsModel::ablCols;

AblScore LevelsModel::getColumnAbl( int col )
{
    switch( col ) {
    case kSTRCol: return AblScore::Str;
    case kDEXCol: return AblScore::Dex;
    case kCONCol: return AblScore::Con;
    case kINTCol: return AblScore::Int;
    case kWISCol: return AblScore::Wis;
    case kCHACol: return AblScore::Cha;
    default: throw std::invalid_argument( "invalid column" );
    }
}

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
        case kSavesCol: return QString("Saves");
        case kFeatsGainedCol: return QString("Gained");
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
    if( !index.isValid() ) {
        return QVariant();
    }

    // Row not inside level range
    if( index.row() >= nwnCharGen->getCharacter()->getNumLevels() ) {
        return QVariant();
    }

    const auto nwnRules = nwnCharGen->getRules();
    const auto nwnChar = nwnCharGen->getCharacter();

    const auto lvl = index.row();
    if( role == Qt::DisplayRole ) {
        const auto col = index.column();
        switch( col ) {
        case kLevelCol:
            return lvl + 1;

        case kClassCol:
            return QVariant( nwnChar->getLevel( index.row() ).c_str() );

        case kHpCol:
            return QVariant( nwnRules->getHpAtLvl( nwnChar, lvl ) );

        case kBabCol:
            return QVariant( QString("+%1").arg( nwnRules->getBabAtLvl( nwnChar, lvl ) ) );

        case kSTRCol:
        case kDEXCol:
        case kCONCol:
        case kINTCol:
        case kWISCol:
        case kCHACol:
            return QVariant( nwnRules->getAblAtLvl( nwnChar, getColumnAbl( col ), lvl ) );

        case kSavesCol: {
                const auto saves = nwnRules->getSavesAtLvl( nwnChar, lvl );
                return QVariant( QString( "%1 / %2 / %3" ).arg( saves.Fort ).arg( saves.Ref ).arg( saves.Will ) );
            }

        case kFeatsGainedCol:
            return index.row() == 0 ? QVariant( "\n- Militia\n- Tower Shield Proficiency\n- Weapon Proficiency (Martial)\n- Shield Proficiency\n- Armor Proficiency (Heavy)\n- Armor Proficiency (Medium)\n- Armor Proficiency (Light)\n- Weapon Proficiency (Simple)" ) : QVariant();

        default:
            break;
        }
    }
    else if( role == Qt::TextAlignmentRole ) {
        return (index.column() == kFeatsGainedCol) ? Qt::AlignLeft : Qt::AlignCenter;
    }

    return QVariant();
}

void LevelsModel::updateView()
{
    emit layoutAboutToBeChanged();
    emit layoutChanged();
}
