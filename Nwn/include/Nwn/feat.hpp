#ifndef NWNFEAT_H
#define NWNFEAT_H

#include <Nwn/base.hpp>
#include <string>
#include <unordered_map>
#include <map>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/unique_ptr.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/map.hpp>

namespace Nwn {

static constexpr int INVALID_FEAT_ID = -1;

/// Gives bonus feat at lvl 1, apparently hardcoded in the game engine
static constexpr int FEAT_ID_QUICK_TO_MASTER = 258;

class AblBlock;

enum class FeatEffectType
{
    InvalidEffect,
    FirstLvlBonusFeat,
    HpBonus,
    FortSave,
    RefSave,
    WillSave,
    StrBonus,
    DexBonus,
    ConBonus,
    IntBonus,
    WisBonus,
    ChaBonus
};

using FeatEffectMap = std::map<FeatEffectType, int>;

class Feat
{
public:
    explicit Feat( int id = -1, const std::string& n = "" );
    ~Feat();

    int getId() const { return id; }
    void setId( int id ) { this->id = id; }

    const std::string& getName() const { return name; }
    void setName( const std::string& n ) { name = n; }

    const std::string& getDescription() const { return description; }
    void setDescription( const std::string& d ) { description = d; }

    bool hasColumn(const std::string& col) const;
    int getColumn(const std::string& col) const;
    void setColumn(const std::string& col, int val);

    void addEffect(FeatEffectType type, int val);
    bool hasEffect(FeatEffectType type) const;
    int getEffect(FeatEffectType type) const;

private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize( Archive & ar, const unsigned int /* file_version */ )
    {
        ar & boost::serialization::make_nvp( "id", id )
           & boost::serialization::make_nvp( "name", name )
           & boost::serialization::make_nvp( "description", description )
           & boost::serialization::make_nvp( "columns", columns )
           & boost::serialization::make_nvp( "effects", effects );
    }

    int id = INVALID_FEAT_ID;
    std::string name;
    std::string description;
    std::unordered_map<std::string, int> columns;
    FeatEffectMap effects;
};

} // namespace Nwn

#endif // NWNFEAT_H
