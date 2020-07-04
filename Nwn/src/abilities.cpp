#include <stdexcept>

#include <Nwn/base.hpp>

namespace Nwn {

AblScore indexToAbl( int i )
{
    return static_cast< AblScore >( i );
}

int getAblMod( int score )
{
    return ( score / 2 ) - 5;
}

} // namespace Nwn
