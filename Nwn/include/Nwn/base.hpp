#ifndef NWNBASE_H
#define NWNBASE_H

#include <array>

#include <boost/bimap.hpp>
#include <boost/assign.hpp>
#include <boost/serialization/nvp.hpp>

namespace Nwn {

enum class AblScore : int {
    Str,
    Dex,
    Con,
    Int,
    Wis,
    Cha
};

AblScore indexToAbl( int i );

int getAblMod( int score );

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

enum class Dice : int {
    d4 = 4,
    d6 = 6,
    d8 = 8,
    d10 = 10,
    d12 = 12,
    d20 = 20
};

/// Integer equivalent of a dice
int diceToInt( Dice d );

/// Integer to enum
Dice intToDice( int i );

struct SavingThrows
{
    template<class Archive>
    void serialize( Archive & ar, const unsigned int /* file_version */ ) {
        ar & boost::serialization::make_nvp( "Fort", Fort )
           & boost::serialization::make_nvp( "Ref", Ref )
           & boost::serialization::make_nvp( "Will", Will );
    }

    SavingThrows& operator+=( const SavingThrows& rhs ) {
        this->Fort += rhs.Fort;
        this->Ref += rhs.Ref;
        this->Will += rhs.Will;
        return *this;
    }

    int Fort { 0 };
    int Ref  { 0 };
    int Will { 0 };
};

} // namespace Nwn

#endif // NWNBASE_H
