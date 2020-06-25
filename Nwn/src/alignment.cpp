#include <stdexcept>

#include <Nwn/base.hpp>

namespace Nwn {

const std::string& alignmentToStr( Alignment aln )
{
    return alignmentStrings.left.at( aln );
}

Alignment strToAlignment( const std::string& txt )
{
    return alignmentStrings.right.at( txt );
}

size_t alignmentToIndex( Alignment aln )
{
    switch( aln ) {
    case Alignment::LawfulGood:     return 0;
    case Alignment::NeutralGood:    return 1;
    case Alignment::ChaoticGood:    return 2;
    case Alignment::LawfulNeutral:  return 3;
    case Alignment::Neutral:        return 4;
    case Alignment::ChaoticNeutral: return 5;
    case Alignment::LawfulEvil:     return 6;
    case Alignment::NeutralEvil:    return 7;
    case Alignment::ChaoticEvil:    return 8;
    default: throw std::invalid_argument( "invalid alignment" );
    }
}

Alignment indexToAlignment( size_t index )
{
    return allAlignments[ index ];
}

} // namespace Nwn
