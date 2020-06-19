#ifndef NWNRULES_H
#define NWNRULES_H

#include <unordered_map>
#include <memory>

#include <Nwn/base.hpp>

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

    void addRace( const Race& r );
    RacesConstLooper getRaces() const { return RacesConstLooper( races ); }
    const Race& getRaceByName( const std::string& name ) const { return *( races.at( name ) ); }
    bool isRaceValid( const std::string& name ) const { return races.find( name ) != races.end(); }

    AblBlock getAdjustedAbls( const Character& chr ) const;

private:
    RaceContainer races;
};

} // namespace Nwn

#endif // NWNRULES_H
