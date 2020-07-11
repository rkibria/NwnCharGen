#include <cassert>

#include <Nwn/base.hpp>

namespace Nwn {

int getBabAtLvl( BabProgression prg, int lvl )
{
    if( lvl < 0 ) {
        return 0;
    }
    switch (prg) {
    case BabProgression::low: return ( lvl + 1 ) / 2;
    case BabProgression::high: return ( lvl + 1 );
    case BabProgression::medium:
        assert( lvl < 30 );
        return mediumBabProgress[ lvl ];
    default:
        return 0;
    }
}

} // namespace Nwn
