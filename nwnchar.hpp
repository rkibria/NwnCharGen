#ifndef NWNCHAR_H
#define NWNCHAR_H

#include <nwnbase.hpp>
#include <array>

namespace Nwn {

class Character
{
public:
    explicit Character();
    ~Character();

    int getPointsRemain() const {return ablPointsRemain;}

    int getAbl(AblScore) const;
    int getAblMod(AblScore) const;

    void decAbl(AblScore);
    void incAbl(AblScore);

private:
    int incCost(int curScore) const;

    static const int ablPointBuy = 32;
    static const int minAblScore = 8;
    static const int maxAblScore = 18;

    int ablPointsRemain;
    std::array<int, 6> abls;
};

} // namespace Nwn

#endif // NWNCHAR_H
