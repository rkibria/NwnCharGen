#include <cassert>

#include <nwnrace.hpp>
#include <nwnrules.hpp>
#include <nwnablblock.hpp>
#include <nwnchar.hpp>
#include <nwnrace.hpp>

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

} // namespace Nwn
