#ifndef NWNBASE_H
#define NWNBASE_H

#include <array>

#include <boost/bimap.hpp>
#include <boost/assign.hpp>

namespace Nwn {

enum class AblScore : int {
    Str,
    Dex,
    Con,
    Int,
    Wis,
    Cha
};

enum class Alignment : int {
    LawfulGood = 1,
    NeutralGood = 2,
    ChaoticGood = 4,
    LawfulNeutral = 8,
    Neutral = 16,
    ChaoticNeutral = 32,
    LawfulEvil = 64,
    NeutralEvil = 128,
    ChaoticEvil = 256
};

static const std::array<Alignment, 9> allAlignments = {
    Alignment::LawfulGood,
    Alignment::NeutralGood,
    Alignment::ChaoticGood,
    Alignment::LawfulNeutral,
    Alignment::Neutral,
    Alignment::ChaoticNeutral,
    Alignment::LawfulEvil,
    Alignment::NeutralEvil,
    Alignment::ChaoticEvil
};

/// Returns order as in allAlignments
Alignment indexToAlignment( size_t index );

/// Returns order as in allAlignments
size_t alignmentToIndex( Alignment aln );

using AlignmentBimap = boost::bimap< Alignment, std::string >;

static const AlignmentBimap alignmentStrings = boost::assign::list_of< AlignmentBimap::relation >
    ( Alignment::LawfulGood, "Lawful Good" )
    ( Alignment::NeutralGood, "Neutral Good" )
    ( Alignment::ChaoticGood, "Chaotic Good" )
    ( Alignment::LawfulNeutral, "Lawful Neutral" )
    ( Alignment::Neutral, "Neutral" )
    ( Alignment::ChaoticNeutral, "Chaotic Neutral" )
    ( Alignment::LawfulEvil, "Lawful Evil" )
    ( Alignment::NeutralEvil, "Neutral Evil" )
    ( Alignment::ChaoticEvil, "Chaotic Evil" )
    ;

const std::string& alignmentToStr( Alignment aln );
Alignment strToAlignment( const std::string& txt );

} // namespace Nwn

#endif // NWNBASE_H
