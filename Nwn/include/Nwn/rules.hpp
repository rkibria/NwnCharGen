#ifndef NWNRULES_H
#define NWNRULES_H

#include <unordered_map>
#include <memory>

#include <Nwn/base.hpp>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/unique_ptr.hpp>

namespace Nwn {

class Race;
class Character;
class AblBlock;

using RaceContainer = std::unordered_map<std::string, std::unique_ptr<Race>>;

class RacesConstItr
{
public:
    const Race& operator*() const { return *( (*itr).second ); }
    bool operator!=( const RacesConstItr &b ) const { return itr != b.itr; }
    RacesConstItr& operator++() {
        ++itr;
        return *this;
    }
private:
    explicit RacesConstItr( RaceContainer::const_iterator i ) :
      itr{ i }
    {}
    RaceContainer::const_iterator itr;
    friend class RacesConstLooper;
};

class RacesConstLooper
{
public:
    RacesConstItr begin() { return RacesConstItr( races.cbegin() ); }
    RacesConstItr end() { return RacesConstItr( races.cend() ); }
private:
    explicit RacesConstLooper( const RaceContainer& r ) :
      races{ r }
    {}
    const RaceContainer& races;
    friend class Rules;
};

class Rules
{
public:
    explicit Rules();
    ~Rules();

    /* Races */
    void setRace( std::unique_ptr<Nwn::Race> r );
    RacesConstLooper getRaces() const { return RacesConstLooper( races ); }
    const Race& getRaceByName( const std::string& name ) const { return *( races.at( name ) ); }
    bool isRaceValid( const std::string& name ) const { return races.find( name ) != races.end(); }

    /* Character methods */
    AblBlock getAdjustedAbls( const Character& chr ) const;

    /* Serialization */
    void save( const char* fileName ) const;
    void restore( const char* fileName );

private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize( Archive & ar, const unsigned int /* file_version */ )
    {
        ar & boost::serialization::make_nvp( "races", races );
    }

    RaceContainer races;
};

} // namespace Nwn

#endif // NWNRULES_H
