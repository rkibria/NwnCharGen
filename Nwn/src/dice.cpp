#include <stdexcept>

#include <Nwn/base.hpp>

namespace Nwn {

int diceToInt( Dice d )
{
    return static_cast<int>( d );
}

Dice intToDice( int i )
{
    switch( i ) {
    case 4: return Dice::d4;
    case 6: return Dice::d6;
    case 8: return Dice::d8;
    case 10: return Dice::d10;
    case 12: return Dice::d12;
    case 20: return Dice::d20;
    default:
        throw std::runtime_error( "Invalid dice int" );
    }
    return Dice::d4;
}

} // namespace Nwn
