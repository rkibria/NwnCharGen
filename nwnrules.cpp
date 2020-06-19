#include <cassert>

#include <nwnrace.hpp>
#include <nwnrules.hpp>

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
