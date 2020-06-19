#include <nwnchar.hpp>
#include <iostream>

namespace Nwn {

Character::Character() :
    ablPointsRemain( ablPointBuy ),
    abls{ minAblScore, minAblScore, minAblScore, minAblScore, minAblScore, minAblScore }
{

}

Character::~Character()
{
}

int Character::getAbl(AblScore abl) const
{
    return abls[ static_cast<int>(abl) ];
}

int Character::getAblMod(AblScore abl) const
{
    const auto score = getAbl( abl );
    auto mod = ( score / 2 ) - 5;
    return mod;
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

void Character::incAbl(AblScore abl)
{
    if( getAbl( abl ) < maxAblScore ) {
        const auto cost = incCost( getAbl( abl ) );
        if( cost <= ablPointsRemain ) {
            ablPointsRemain -= cost;
            ++abls[ static_cast<int>( abl ) ];
        }
    }
}

void Character::decAbl(AblScore abl)
{
    if( getAbl( abl ) > minAblScore ) {
        const auto cost = incCost( getAbl( abl ) - 1 );
        ablPointsRemain += cost;
        --abls[ static_cast<int>( abl ) ];
    }
}

} // namespace NwnChar
