#ifndef NWNCHAR_H
#define NWNCHAR_H

#include <nwnbase.hpp>
#include <array>
#include <string>
#include <memory>

namespace Nwn {

class AblBlock;

class Character
{
public:
    explicit Character();
    ~Character();

    /* Abilities  */
    AblBlock& getAbls() { return *abls; }
    const AblBlock& getAbls() const { return *abls; }

    int getPointsRemain() const { return ablPointsRemain; }
    void decAbl( AblScore );
    void incAbl( AblScore );

    /* Race */
    std::string getRace() const { return race; }
    void setRace( const std::string& r ) { race = r; }

private:
    int incCost( int curScore ) const;

    static const int ablPointBuy;
    static const int minAblScore;
    static const int maxAblScore;

    int ablPointsRemain;
    std::unique_ptr< AblBlock > abls;

    std::string race;
};

} // namespace Nwn

#endif // NWNCHAR_H
