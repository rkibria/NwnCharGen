#ifndef NWNCHAR_H
#define NWNCHAR_H

#include <nwnbase.hpp>
#include <array>
#include <string>

namespace Nwn {

class Character
{
public:
    explicit Character();
    ~Character();

    /* Abilities  */

    int getPointsRemain() const { return ablPointsRemain; }
    int getAbl(AblScore) const;
    int getAblMod(AblScore) const;
    void decAbl(AblScore);
    void incAbl(AblScore);

    /* Race */
    std::string getRace() const { return race; }
    void setRace(const std::string& r) { race = r; }

private:
    int incCost(int curScore) const;

    static const int ablPointBuy = 32;
    static const int minAblScore = 8;
    static const int maxAblScore = 18;

    int ablPointsRemain;
    std::array<int, 6> abls;

    std::string race;
};

} // namespace Nwn

#endif // NWNCHAR_H
