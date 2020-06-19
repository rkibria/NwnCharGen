#ifndef NWNRULES_H
#define NWNRULES_H

#include <unordered_map>
#include <memory>

#include <nwnbase.hpp>

namespace Nwn {

class Race;

using RaceContainer = std::unordered_map<std::string, std::unique_ptr<Race>>;

class RacesConstItr {
public:
    RacesConstItr& operator++() {
        ++itr;
        return *this;
    }
    const Race& operator*() const { return *( (*itr).second ); }
    bool end() const { return itr == races.cend(); }

private:
    explicit RacesConstItr( const RaceContainer& r ) :
      races{ r },
      itr{ r.cbegin() }
    {}

    const RaceContainer& races;
    RaceContainer::const_iterator itr;

    friend class Rules;
};

class Rules
{
public:
    explicit Rules();
    ~Rules();

    void addRace( const Race& r );
    RacesConstItr getRaces() const { return RacesConstItr( races ); }
    const Race& getRaceByName( const std::string& name ) { return *( races.at( name ) ); }

private:
    RaceContainer races;
};

} // namespace Nwn

#endif // NWNRULES_H
