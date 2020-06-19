#ifndef NWNRULES_H
#define NWNRULES_H

#include <unordered_map>

#include <nwnbase.hpp>
#include <nwnrace.hpp>

namespace Nwn {

class RaceConstItr {
public:
    void next() {++itr;}
    const Race& get() const {return (*itr).second;}
    bool end() const {return itr == races.cend();}

private:
    explicit RaceConstItr(const std::unordered_map<std::string, Race>& r) :
      races{r},
      itr{r.cbegin()}
    {}

    const std::unordered_map<std::string, Race>& races;
    std::unordered_map<std::string, Race>::const_iterator itr;

    friend class Rules;
};

class Rules
{
public:
    explicit Rules();
    ~Rules();

    void addRace(const Race& r);
    RaceConstItr getRacesConstItr() const { return RaceConstItr(races); }
    const Race& getRaceByName( const std::string& name ) { return races.at( name ); }

private:
    std::unordered_map<std::string, Race> races;
};

} // namespace Nwn

#endif // NWNRULES_H
