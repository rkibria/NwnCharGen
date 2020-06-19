#include <cassert>

#include <nwnrules.hpp>
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

} // namespace Nwn
