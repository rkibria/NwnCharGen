#include <Nwn/chclass.hpp>

namespace Nwn {

ChClass::ChClass( int id, const std::string& n ) :
    id{ id },
    name{ n },
    featsPerLvl{ std::make_unique< FeatsPerLevelMap >() },
    bonusFeats{ std::make_unique< std::vector< bool > >() },
    bonusChoices{ std::make_unique< BonusFeatsSet >() },
    exclusiveBonusChoices{ std::make_unique< BonusFeatsSet >() }
{
}

ChClass::~ChClass()
{
}

void ChClass::setFeatsPerLvl( std::unique_ptr< FeatsPerLevelMap > fpl )
{
    featsPerLvl = std::move( fpl );
}

const FeatsPerLevelMap& ChClass::getFeatsPerLvl() const
{
    return *featsPerLvl;
}

void ChClass::setBonusFeats( std::unique_ptr< std::vector< bool > > bf )
{
    bonusFeats = std::move( bf );
}

bool ChClass::getBonusFeat( int lvl ) const
{
    return (*bonusFeats)[ lvl ];
}

void ChClass::setBonusChoices( std::unique_ptr< BonusFeatsSet > bc )
{
    bonusChoices = std::move( bc );
}

const BonusFeatsSet& ChClass::getBonusChoices() const
{
    return *bonusChoices;
}

void ChClass::setExclusiveBonusChoices( std::unique_ptr< BonusFeatsSet > bc )
{
    exclusiveBonusChoices = std::move( bc );
}

const BonusFeatsSet& ChClass::getExclusiveBonusChoices() const
{
    return *exclusiveBonusChoices;
}

} // namespace Nwn
