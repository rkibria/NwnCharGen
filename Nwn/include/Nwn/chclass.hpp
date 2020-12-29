#ifndef NWNCHCLASS_H
#define NWNCHCLASS_H

#include <Nwn/base.hpp>
#include <string>
#include <vector>
#include <array>
#include <memory>
#include <map>
#include <set>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/array.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/unique_ptr.hpp>

namespace Nwn {

using FeatsPerLevelMap = std::map< int, std::set< int > >;

class ChClass
{
public:
    explicit ChClass( const std::string& n = "" );
    ~ChClass();
    ChClass( const ChClass& );

    const std::string& getName() const { return name; }
    void setName( const std::string& n ) { name = n; }

    const std::string& getDescription() const { return description; }
    void setDescription( const std::string& d ) { description = d; }

    Dice getHitDie() const { return hitDie; }
    void setHitDie( Dice d ) { hitDie = d; }

    void setBabProgression( const std::vector<int>& prg ) { babProgression = prg; }
    int getBabAtLvl( int lvl ) const {
        return ( static_cast< size_t >( lvl ) < babProgression.size() ) ? babProgression[ lvl ] : 0;
    }

    void setSaves( const std::vector< SavingThrows >& values ) { saves = values; }
    const SavingThrows& getSavesAtLvl( int lvl ) const {
        const static SavingThrows nullSaves;
        return ( static_cast< size_t >( lvl ) < saves.size() ) ? saves[ lvl ] : nullSaves;
    }

    void setFeatsPerLvl( std::unique_ptr< FeatsPerLevelMap > fpl );
    const FeatsPerLevelMap& getFeatsPerLvl() const;

    void setBonusFeats( std::unique_ptr< std::vector< bool > > bf );
    bool getBonusFeat( int lvl ) const;

private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize( Archive & ar, const unsigned int /* file_version */ ) {
        ar & boost::serialization::make_nvp( "name", name )
           & boost::serialization::make_nvp( "description", description )
           & boost::serialization::make_nvp( "hitDie", hitDie )
           & boost::serialization::make_nvp( "babProgression", babProgression )
           & boost::serialization::make_nvp( "saves", saves )
           & boost::serialization::make_nvp( "featsPerLvl", featsPerLvl )
           & boost::serialization::make_nvp( "bonusFeats", bonusFeats );
    }

    std::string name;
    std::string description;
    Dice hitDie{ Dice::d4 };
    std::vector< int > babProgression;
    std::vector< SavingThrows > saves;
    std::unique_ptr< FeatsPerLevelMap > featsPerLvl;
    std::unique_ptr< std::vector< bool > > bonusFeats;
};

} // namespace Nwn

#endif // NWNCHCLASS_H
