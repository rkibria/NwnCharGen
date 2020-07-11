#include <stdexcept>

#include <Nwn/base.hpp>

namespace Nwn {

int getBabAtLvl( BabProgression prg, int lvl )
{
    assert( lvl >= 0 && lvl < 30 );
    switch (prg) {
    case BabProgression::low: return ( lvl + 1 ) / 2;
    case BabProgression::high: return ( lvl + 1 );
    case BabProgression::medium: return mediumBabProgress[ lvl ];
    default:
        return 0;
    }
}

} // namespace Nwn
