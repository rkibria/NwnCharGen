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

using RaceContainer = std::unordered_map< std::string, std::unique_ptr< Race > >;
using ChClassContainer = std::unordered_map< std::string, std::unique_ptr< ChClass > >;

class Rules
{
public:
    explicit Rules();
    ~Rules();

    // RACES
    void setRace( std::unique_ptr< Nwn::Race > r );
    ConstMapLooper< Race, RaceContainer > getRaces() const { return ConstMapLooper< Race, RaceContainer >( races ); }
    const Race& getRaceByName( const std::string& name ) const { return *( races.at( name ) ); }
    bool isRaceValid( const std::string& name ) const { return races.find( name ) != races.end(); }
    void removeRace( const std::string& name );

    // CHARACTER CLASSES
    void setChClass( std::unique_ptr< Nwn::ChClass > r );
    ConstMapLooper< ChClass, ChClassContainer > getChClasses() const {
        return ConstMapLooper< ChClass, ChClassContainer >( chclasses );
    }
    const ChClass& getChClassByName( const std::string& name ) const { return *( chclasses.at( name ) ); }
    bool isChClassValid( const std::string& name ) const { return chclasses.find( name ) != chclasses.end(); }
    void removeChClass( const std::string& name );

    // CHARACTER METHODS
    int getAblAtLvl( const Character* chr, AblScore abl, int lvl );
    int getHpAtLvl( const Character* chr, int lvl );

    // SERIALIZATION
    void save( const char* fileName ) const;
    void restore( const char* fileName );

private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize( Archive & ar, const unsigned int /* file_version */ ) {
        ar & boost::serialization::make_nvp( "races", races )
           & boost::serialization::make_nvp( "chclasses", chclasses );
    }

    RaceContainer races;
    ChClassContainer chclasses;
};

} // namespace Nwn

#endif // NWNRULES_H
