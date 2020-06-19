#include <cassert>
#include <nwnrules.hpp>

namespace Nwn {

Rules::Rules()
{
}

Rules::~Rules()
{
}

void Rules::addRace(const Race& r)
{
    assert( !r.getName().empty() );
    races.insert( { r.getName(), r } );
}

} // namespace Nwn
