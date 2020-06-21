#include <cassert>
#include <fstream>

#include <Nwn/race.hpp>
#include <Nwn/rules.hpp>
#include <Nwn/ablblock.hpp>
#include <Nwn/character.hpp>
#include <Nwn/race.hpp>

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

namespace Nwn {

Rules::Rules()
{
}

Rules::~Rules()
{
}

void Rules::addRace( const Race& r )
{
    assert( !r.getName().empty() );
    races.insert( { r.getName(), std::make_unique<Race>( r ) } );
}

AblBlock Rules::getAdjustedAbls( const Character& chr ) const
{
    AblBlock result = chr.getAbls();

    const auto& raceName = chr.getRace();
    if( isRaceValid( raceName ) ) {
        const auto& race = getRaceByName( raceName );
        const auto& raceAblAdjusts = race.getAblAdjusts();
        result.add( raceAblAdjusts );
    }

    return result;
}

void Rules::save( const char* fileName ) const
{
    std::ofstream ofs( fileName );
    assert( ofs.good() );
    boost::archive::xml_oarchive oa( ofs );
    oa << boost::serialization::make_nvp( "rules", *this );
}

} // namespace Nwn
