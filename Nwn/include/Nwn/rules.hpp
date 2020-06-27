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

using RaceContainer = std::unordered_map<std::string, std::unique_ptr<Race>>;

class Rules
{
public:
    explicit Rules();
    ~Rules();

    /* Races */
    void setRace( std::unique_ptr<Nwn::Race> r );
    ConstMapLooper< Race, RaceContainer > getRaces() const { return ConstMapLooper< Race, RaceContainer >( races ); }
    const Race& getRaceByName( const std::string& name ) const { return *( races.at( name ) ); }
    bool isRaceValid( const std::string& name ) const { return races.find( name ) != races.end(); }
    void removeRace( const std::string& name );

    /* Character methods */
    AblBlock getAdjustedAbls( const Character& chr ) const;

    /* Serialization */
    void save( const char* fileName ) const;
    void restore( const char* fileName );

private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize( Archive & ar, const unsigned int /* file_version */ ) {
        ar & boost::serialization::make_nvp( "races", races );
    }

    RaceContainer races;
};

} // namespace Nwn

#endif // NWNRULES_H
