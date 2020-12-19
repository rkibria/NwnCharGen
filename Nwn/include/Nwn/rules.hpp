#ifndef NWNRULES_H
#define NWNRULES_H

#include <unordered_map>
#include <memory>

#include <Nwn/base.hpp>
#include <Nwn/constmaplooper.hpp>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/unique_ptr.hpp>

namespace Nwn {

class Race;
class Character;
class AblBlock;
class ChClass;
class Feat;

using RaceContainer = std::unordered_map< std::string, std::unique_ptr< Race > >;
using ChClassContainer = std::unordered_map< std::string, std::unique_ptr< ChClass > >;
using FeatContainer = std::unordered_map< int, std::unique_ptr< Feat > >;

class Rules
{
public:
    explicit Rules();
    ~Rules();

    // RACES
    void setRace( std::unique_ptr< Nwn::Race > r );
    ConstMapLooper< Race, RaceContainer > getRaces() const { return ConstMapLooper< Race, RaceContainer >( races ); }
    const Race* getRaceByName( const std::string& name ) const;
    bool isRaceValid( const std::string& name ) const;
    void removeRace( const std::string& name );

    // CHARACTER CLASSES
    void setChClass( std::unique_ptr< Nwn::ChClass > r );
    ConstMapLooper< ChClass, ChClassContainer > getChClasses() const {
        return ConstMapLooper< ChClass, ChClassContainer >( chclasses );
    }
    const ChClass* getChClassByName( const std::string& name ) const;
    bool isChClassValid( const std::string& name ) const;
    void removeChClass( const std::string& name );

    // FEATS
    const Nwn::Feat* getFeat( int id ) const;
    void setFeat( std::unique_ptr< Nwn::Feat > f );

    // CHARACTER METHODS
    int getAblAtLvl( const Character* chr, AblScore abl, int lvl ) const;
    int getHpAtLvl( const Character* chr, int lvl ) const;
    int getBabAtLvl( const Character* chr, int lvl ) const;
    SavingThrows getSavesAtLvl( const Character* chr, int lvl ) const;

    /// Only at that level, not including earlier ones
    std::set< int > getFeatsGainedAtLvl( const Character* chr, int lvl ) const;

    /// Only at that level, not including earlier ones
    int getNumFeatChoicesAtLvl( const Character* chr, int lvl ) const;

    // SERIALIZATION
    void save( const char* fileName ) const;
    void restore( const char* fileName );

private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize( Archive & ar, const unsigned int /* file_version */ ) {
        ar & boost::serialization::make_nvp( "races", races )
           & boost::serialization::make_nvp( "chclasses", chclasses )
           & boost::serialization::make_nvp( "feats", feats );
    }

    RaceContainer races;
    ChClassContainer chclasses;
    FeatContainer feats;
};

} // namespace Nwn

#endif // NWNRULES_H
