#include <Nwn/char.hpp>
#include <Nwn/ablblock.hpp>
#include <iostream>

namespace Nwn {

const int Character::ablPointBuy = 32;
const int Character::minAblScore = 8;
const int Character::maxAblScore = 18;

Character::Character() :
    ablPointsRemain( ablPointBuy ),
    abls{ std::make_unique< AblBlock >( minAblScore ) },
    race{}
{

}

Character::~Character()
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

} // namespace Nwn
