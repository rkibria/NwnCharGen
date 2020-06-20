#ifndef NWNBASE_H
#define NWNBASE_H

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
    LawfulGood,
    NeutralGood,
    ChaoticGood,
    LawfulNeutral,
    Neutral,
    ChaoticNeutral,
    LawfulEvil,
    NeutralEvil,
    ChaoticEvil
};

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

} // namespace Nwn

#endif // NWNBASE_H
