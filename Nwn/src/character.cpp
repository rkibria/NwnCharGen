#include <iostream>
#include <fstream>

#include <Nwn/character.hpp>
#include <Nwn/ablblock.hpp>

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

namespace Nwn {

const int Character::ablPointBuy;
const int Character::minAblScore;
const int Character::maxAblScore;

Character::Character() :
    abls{ std::make_unique< AblBlock >( minAblScore ) }
{
}

Character::~Character() noexcept
{
}

int Character::incCost(int curScore) const
{
    switch( curScore ) {
    case 8: return 1;
    case 9: return 1;
    case 10: return 1;
    case 11: return 1;
    case 12: return 1;
    case 13: return 1;
    case 14: return 2;
    case 15: return 2;
    case 16: return 3;
    case 17: return 3;

    default:
        std::cerr << "ability range exceeded\n";
        return 0;
    }
}

void Character::incAbl( AblScore abl )
{
    const auto curValue = abls->getAbl( abl );
    if( curValue < maxAblScore ) {
        const auto cost = incCost( curValue );
        if( cost <= ablPointsRemain ) {
            ablPointsRemain -= cost;
            abls->setAbl( abl, curValue + 1);
        }
    }
}

void Character::decAbl( AblScore abl )
{
    const auto curValue = abls->getAbl( abl );
    if( curValue > minAblScore ) {
        const auto cost = incCost( curValue - 1 );
        ablPointsRemain += cost;
        abls->setAbl( abl, curValue - 1);
    }
}

void Character::save( const char* fileName ) const
{
    std::ofstream ofs( fileName );
    assert( ofs.good() );
    boost::archive::xml_oarchive oa( ofs );
    oa << boost::serialization::make_nvp( "character", *this );
}

void Character::restore( const char* fileName )
{
    std::ifstream ifs( fileName );
    assert( ifs.good() );
    boost::archive::xml_iarchive ia( ifs );
    ia >> boost::serialization::make_nvp( "character", *this );
}

} // namespace Nwn
