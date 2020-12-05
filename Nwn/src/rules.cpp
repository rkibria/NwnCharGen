#include <cassert>
#include <fstream>

#include <Nwn/race.hpp>
#include <Nwn/rules.hpp>
#include <Nwn/ablblock.hpp>
#include <Nwn/character.hpp>
#include <Nwn/race.hpp>
#include <Nwn/chclass.hpp>
#include <Nwn/feat.hpp>

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

namespace Nwn {

Rules::Rules()
{
}

Rules::~Rules()
{
}

// RACES

void Rules::setRace( std::unique_ptr<Nwn::Race> r )
{
    assert( !r->getName().empty() );
    races[ r->getName() ] = std::move( r );
}

void Rules::removeRace( const std::string& name )
{
    if( isRaceValid( name ) ) {
        races.erase( name );
    }
}

// CHARACTER CLASSES

void Rules::setChClass( std::unique_ptr< Nwn::ChClass > cc )
{
    assert( !cc->getName().empty() );
    chclasses[ cc->getName() ] = std::move( cc );
}

void Rules::removeChClass( const std::string& name )
{
    if( isChClassValid( name ) ) {
        chclasses.erase( name );
    }
}

// FEATS

void Rules::setFeat( std::unique_ptr< Nwn::Feat > f )
{
    feats[ f->getId()] = std::move( f );
}

// CHARACTER METHODS

int Rules::getAblAtLvl( const Character* chr, AblScore abl, int lvl )
{
    int ablVal = chr->getAbls().getAbl( abl );

    const auto& raceName = chr->getRace();
    if( isRaceValid( raceName ) ) {
        const auto& race = getRaceByName( raceName );
        ablVal += race.getAblAdjusts().getAbl( abl );
    }

    switch( lvl ) {
    case 29:
    case 28:
    case 27:
        ablVal += ( chr->getAblInc( 6 ) == abl ) ? 1 : 0;
    case 26:
    case 25:
    case 24:
    case 23:
        ablVal += ( chr->getAblInc( 5 ) == abl ) ? 1 : 0;
    case 22:
    case 21:
    case 20:
    case 19:
        ablVal += ( chr->getAblInc( 4 ) == abl ) ? 1 : 0;
    case 18:
    case 17:
    case 16:
    case 15:
        ablVal += ( chr->getAblInc( 3 ) == abl ) ? 1 : 0;
    case 14:
    case 13:
    case 12:
    case 11:
        ablVal += ( chr->getAblInc( 2 ) == abl ) ? 1 : 0;
    case 10:
    case 9:
    case 8:
    case 7:
        ablVal += ( chr->getAblInc( 1 ) == abl ) ? 1 : 0;
    case 6:
    case 5:
    case 4:
    case 3:
        ablVal += ( chr->getAblInc( 0 ) == abl ) ? 1 : 0;
    case 2:
    case 1:
    case 0:
    default:
        break;
    }

    return ablVal;
}

namespace  {

/// Iteration limit for levels: the lower of level and character's class levels
int getLvlItrLimit( const Character* chr, int lvl )
{
    return std::min( lvl, chr->getNumLevels() - 1 );
}

} // namespace

int Rules::getHpAtLvl( const Character* chr, int lvl )
{
    int hp = 0;
    const auto conBonus = getAblMod( getAblAtLvl( chr, AblScore::Con, lvl ) );
    for( int i = 0; i <= getLvlItrLimit( chr, lvl ); ++i ) {
        const auto& lvlClass = chr->getLevel( i );
        if( isChClassValid( lvlClass ) ) {
            const auto& chClass = getChClassByName( lvlClass );
            hp += diceToInt( chClass.getHitDie() );
            hp += conBonus;
        }
    }
    return hp;
}

int Rules::getBabAtLvl( const Character* chr, int lvl )
{
    const auto chclassLevels = chr->getChClassCountsAtLvl( lvl );
    int bab = 0;
    for( auto itr = chclassLevels.cbegin(); itr != chclassLevels.cend(); ++itr ) {
        const auto& chClassName = itr->first;
        const auto lvls = itr->second;
        assert( ! chClassName.empty() );
        assert( lvls > 0 );
        if( isChClassValid( chClassName ) ) {
            const auto& chClass = getChClassByName( chClassName );
            bab += chClass.getBabAtLvl( lvls - 1 );
        }
    }
    return bab;
}

SavingThrows Rules::getSavesAtLvl( const Character* chr, int lvl )
{
    const auto chclassCounts = chr->getChClassCountsAtLvl( lvl );
    SavingThrows sav;
    for( auto itr = chclassCounts.cbegin(); itr != chclassCounts.cend(); ++itr ) {
        const auto& chClassName = itr->first;
        const auto lvls = itr->second;
        assert( ! chClassName.empty() );
        assert( lvls > 0 );
        if( isChClassValid( chClassName ) ) {
            const auto& chClass = getChClassByName( chClassName );
            const auto& chClassSaves = chClass.getSavesAtLvl( lvls - 1 );
            sav += chClassSaves;
        }
    }

    sav.Fort += getAblMod( getAblAtLvl( chr, AblScore::Con, lvl ) );
    sav.Ref += getAblMod( getAblAtLvl( chr, AblScore::Dex, lvl ) );
    sav.Will += getAblMod( getAblAtLvl( chr, AblScore::Wis, lvl ) );
    return sav;
}

// SERIALIZATION

void Rules::save( const char* fileName ) const
{
    std::ofstream ofs( fileName );
    assert( ofs.good() );
    boost::archive::xml_oarchive oa( ofs );
    oa << boost::serialization::make_nvp( "rules", *this );
}

void Rules::restore( const char* fileName )
{
    std::ifstream ifs( fileName );
    assert( ifs.good() );
    boost::archive::xml_iarchive ia( ifs );
    ia >> boost::serialization::make_nvp( "rules", *this );
}

} // namespace Nwn
