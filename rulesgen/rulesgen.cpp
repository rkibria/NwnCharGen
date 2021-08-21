#include <iostream>
#include <unordered_map>
#include <stdexcept>
#include <sstream>
#include <set>
#include <map>
#include <ctime>

#include <Precomp.h>
#include <2DAFileReader.h>
#include <TlkFileReader.h>

#include <Nwn/base.hpp>
#include <Nwn/character.hpp>
#include <Nwn/ablblock.hpp>
#include <Nwn/race.hpp>
#include <Nwn/rules.hpp>
#include <Nwn/chclass.hpp>
#include <Nwn/feat.hpp>
using namespace Nwn;

#include <boost/algorithm/string.hpp>

#include "twodamapper.h"

using TlkFileReader16 = TlkFileReader<NWN::ResRef16>;

namespace  {

class TlkSwitcher
{
public:
    TlkSwitcher( const TlkFileReader16& main, TlkFileReader16* aux )
        : mainTlk{ main },
          auxTlk{ aux }
    {
    }

    bool GetTalkString( TlkFileReader16::StrRef stringId, std::string& s ) const {
        const auto customTlkIndex = 16777216;
        if( stringId <= customTlkIndex ) {
            return mainTlk.GetTalkString( stringId, s );
        }
        else {
            assert( auxTlk );
            return auxTlk->GetTalkString( stringId - customTlkIndex, s );
        }
    }

private:
    const TlkFileReader16& mainTlk;
    const TlkFileReader16* auxTlk;
};

std::string translateToNwn2Tags( const std::string& text )
{
    std::stringstream inStrm( text );
    std::stringstream outStrm;

    std::string line;
    while( std::getline( inStrm, line, '\n' ) ){
        outStrm << line << " <br>";
    }

    return outStrm.str();
}

const std::vector< SavingThrows >& loadSavesTable( const std::string& tableName, TwoDAMapper& twodaMapper )
{
    static const std::vector< SavingThrows > empty = {};

    static std::unordered_map< std::string, std::vector< SavingThrows > > savesTables;

    constexpr const auto colLevel = "Level";
    constexpr const auto colFort = "FortSave";
    constexpr const auto colRef = "RefSave";
    constexpr const auto colWill = "WillSave";
    const auto lowerName = boost::to_lower_copy( tableName );
    if( savesTables.count( lowerName ) == 0 ) {
        const auto& twoDAPath = twodaMapper.getFile( lowerName );
        if( twoDAPath.empty() ) {
            std::cerr << "WARNING: could not find saving throw 2da file " << lowerName << std::endl;
            return empty;
        }

        TwoDAFileReader cls_savthr_2da( twoDAPath.c_str() );
        const auto nRows = cls_savthr_2da.GetRowCount();
        savesTables.emplace( lowerName, std::vector< SavingThrows >( nRows ) );
        auto& saves = savesTables.at( lowerName );
        for( size_t row = 0 ; row < nRows; ++row ) {
            int level;
            const auto lvlOk = cls_savthr_2da.Get2DAInt( colLevel, row, level );
            assert( lvlOk );

            int fort;
            const auto fortOk = cls_savthr_2da.Get2DAInt( colFort, row, fort );
            assert( fortOk );

            int ref;
            const auto refOk = cls_savthr_2da.Get2DAInt( colRef, row, ref );
            assert( refOk );

            int will;
            const auto willOk = cls_savthr_2da.Get2DAInt( colWill, row, will );
            assert( willOk );

            const auto lvl = level - 1;
            auto& s = saves[ lvl ];
            s.Fort = fort;
            s.Ref = ref;
            s.Will = will;
        }
    }
    assert( savesTables.count( lowerName ) == 1 );
    return savesTables.at( lowerName );
}

const std::vector<int>& loadAttackTable( const std::string& tableName, TwoDAMapper& twodaMapper )
{
    static std::unordered_map< std::string, std::vector<int> > attackTables;

    constexpr const auto colName = "BAB";
    const auto lowerName = boost::to_lower_copy( tableName );
    if( attackTables.count( lowerName ) == 0 ) {
        TwoDAFileReader attack_2da( twodaMapper.getFile( lowerName ).c_str() );
        assert( attack_2da.HasColumn( colName ) );
        assert( attack_2da.GetRowCount() >= Character::maxLevel );
        attackTables.emplace( lowerName, std::vector<int>( Character::maxLevel ) );
        auto& prg = attackTables.at( lowerName );
        for( size_t row = 0 ; row < Character::maxLevel; ++row ) {
            int bab;
            const auto babOk = attack_2da.Get2DAInt( colName, row, bab );
            assert( babOk );
            prg[ row ] = bab;
        }
    }
    assert( attackTables.count( lowerName ) == 1 );
    return attackTables.at( lowerName );
}

std::unique_ptr< std::set<int> > loadRacialFeatsTable( const std::string& tableName, TwoDAMapper& twodaMapper )
{
    auto feats = std::make_unique< std::set<int> >();

    constexpr const auto colName = "FeatIndex";
    const auto lowerName = boost::to_lower_copy( tableName );

    TwoDAFileReader racial_feat_2da( twodaMapper.getFile( lowerName ).c_str() );
    assert( racial_feat_2da.HasColumn( colName ) );

    for( size_t row = 0 ; row < racial_feat_2da.GetRowCount(); ++row ) {
        int feat;
        const auto featOk = racial_feat_2da.Get2DAInt( colName, row, feat );
        if( featOk ) {
            (*feats).insert( feat );
        }
    }

    return feats;
}

std::unique_ptr< std::vector< bool > > loadBonusFeatsTable( const std::string& tableName, TwoDAMapper& twodaMapper )
{
    auto bonusFeats = std::make_unique< std::vector< bool > >();
    bonusFeats->resize( Character::maxLevel );

    constexpr const auto colBonus = "Bonus";

    const auto& filePath = twodaMapper.getFile( tableName );
    if( !filePath.empty() ) {
        TwoDAFileReader class_bfeat_2da( filePath.c_str() );
        assert( class_bfeat_2da.HasColumn( colBonus ) );

        for( size_t row = 0 ; row < class_bfeat_2da.GetRowCount(); ++row ) {
            if( row >= Character::maxLevel ) {
                break;
            }

            int isBonus;
            const auto featOk = class_bfeat_2da.Get2DAInt( colBonus, row, isBonus );
            if( !featOk ) {
                continue;
            }
            (*bonusFeats)[ row ] = isBonus;
        }
    }

    return bonusFeats;
}

std::tuple<
    std::unique_ptr< FeatsPerLevelMap >,
    std::unique_ptr< BonusFeatsSet >,
    std::unique_ptr< BonusFeatsSet >
>
loadClassFeatsTable( const std::string& tableName, TwoDAMapper& twodaMapper )
{
    auto featsPerLevel = std::make_unique< FeatsPerLevelMap >();
    auto bonusFeats = std::make_unique< BonusFeatsSet >();
    auto exclusiveBonusFeats = std::make_unique< BonusFeatsSet >();

    constexpr const auto colFeatIndex = "FeatIndex";
    constexpr const auto colList = "List";
    constexpr const auto colGrantedOnLvl = "GrantedOnLevel";

    TwoDAFileReader class_feat_2da( twodaMapper.getFile( tableName ).c_str() );
    assert( class_feat_2da.HasColumn( colFeatIndex ) );
    assert( class_feat_2da.HasColumn( colList ) );
    assert( class_feat_2da.HasColumn( colGrantedOnLvl ) );

    for( size_t row = 0 ; row < class_feat_2da.GetRowCount(); ++row ) {
        int feat;
        const auto featOk = class_feat_2da.Get2DAInt( colFeatIndex, row, feat );
        if( !featOk ) {
            continue;
        }
        int list;
        const auto listOk = class_feat_2da.Get2DAInt( colList, row, list );
        if( !listOk ) {
            continue;
        }
        int granted;
        const auto grantedOk = class_feat_2da.Get2DAInt( colGrantedOnLvl, row, granted );
        if( !grantedOk ) {
            continue;
        }

        enum featListValues {
            kSelectableOnLvlUp,
            kRegularOrBonusFeat,
            kBonusFeatOnly,
            kAutomaticallyGranted
        };

        if( list == kAutomaticallyGranted ) {
            const auto grantedLvl = granted - 1;
            if( (*featsPerLevel).find( grantedLvl ) == (*featsPerLevel).end() ) {
                (*featsPerLevel)[ grantedLvl ] = std::set<int>();
            }
            (*featsPerLevel)[ grantedLvl ].insert( feat );
        }
        else if( list <= kRegularOrBonusFeat ) {
            (*bonusFeats).insert( feat );
        }
        else if( list == kBonusFeatOnly ) {
            (*exclusiveBonusFeats).insert( feat );
        }
    }

    return std::make_tuple( std::move( featsPerLevel ), std::move( bonusFeats ), std::move( exclusiveBonusFeats ) );
}

void importClasses( Rules &nwnRules, const TlkSwitcher& tlkSw, TwoDAMapper& twodaMapper )
{
    TwoDAFileReader classes_2da( twodaMapper.getFile( "classes" ).c_str() );

    for( size_t row = 0 ; row < classes_2da.GetRowCount(); ++row ) {
        int nameRef, isPlayerClass;
        if( classes_2da.Get2DAInt( "Name", row, nameRef )
                && classes_2da.Get2DAInt( "PlayerClass", row, isPlayerClass )
                && isPlayerClass ) {
            std::string name;
            const auto nameOk = tlkSw.GetTalkString( nameRef, name );
            assert( nameOk );

            if( name.empty() ) {
                std::cout << "importClasses: skipping row " << row << ", name is empty" << std::endl;
                continue;
            }

            int descrRef;
            std::string descr;
            const auto descrOk = classes_2da.Get2DAInt( "Description", row, descrRef );
            assert( descrOk );
            const auto descrRefOk = tlkSw.GetTalkString( descrRef, descr );
            assert( descrRefOk );

            int hitDieInt;
            const auto hitdieOk = classes_2da.Get2DAInt( "HitDie", row, hitDieInt );
            assert( hitdieOk );
            const auto hitDie = intToDice( hitDieInt );

            std::string attackStr;
            const auto attackOk = classes_2da.Get2DAString( "AttackBonusTable", row, attackStr );
            assert( attackOk );
            const auto& prg = loadAttackTable( attackStr, twodaMapper );

            std::string savesStr;
            const auto savesOk = classes_2da.Get2DAString( "SavingThrowTable", row, savesStr );
            assert( savesOk );
            boost::algorithm::to_lower( savesStr );
            const auto& saves = loadSavesTable( savesStr, twodaMapper );
            if( saves.empty() ) {
                std::cerr << "importClasses: skipping class " << name << ", saves table not found" << std::endl;
                continue;
            }

            std::string featsStr;
            const auto featsOk = classes_2da.Get2DAString( "FeatsTable", row, featsStr );
            auto featsPerLvl = std::make_unique< FeatsPerLevelMap >();
            auto bonusChoices = std::make_unique< BonusFeatsSet >();
            auto exclusiveBonusChoices = std::make_unique< BonusFeatsSet >();
            if( featsOk ) {
                boost::algorithm::to_lower( featsStr );
                auto [ tmpFeatsPerLvl, tmpBonusChoices, tmpExclusiveBonusFeats ] = loadClassFeatsTable( featsStr, twodaMapper );
                featsPerLvl = std::move( tmpFeatsPerLvl );
                bonusChoices = std::move( tmpBonusChoices );
                exclusiveBonusChoices = std::move( tmpExclusiveBonusFeats );
            }

            std::string bonusfeatsStr;
            const auto bonusfeatsOk = classes_2da.Get2DAString( "BonusFeatsTable", row, bonusfeatsStr );
            auto bonusFeats = std::make_unique< std::vector< bool > >();
            if( bonusfeatsOk ) {
                boost::algorithm::to_lower( bonusfeatsStr );
                bonusFeats = loadBonusFeatsTable( bonusfeatsStr, twodaMapper );
            }

            std::cout << "importing class " << name << std::endl;
            std::unique_ptr< ChClass > chClass = std::make_unique< ChClass >( row, name );
            chClass->setDescription( translateToNwn2Tags( descr ) );
            chClass->setHitDie( hitDie );
            chClass->setBabProgression( prg );
            chClass->setSaves( saves );
            chClass->setFeatsPerLvl( std::move( featsPerLvl ) );
            chClass->setBonusFeats( std::move( bonusFeats ) );
            chClass->setBonusChoices( std::move( bonusChoices ) );
            chClass->setExclusiveBonusChoices( std::move( exclusiveBonusChoices ) );
            nwnRules.setChClass( std::move( chClass ) );
        }
    }
}

void importRaces( Rules &nwnRules, const TlkSwitcher& tlkSw, TwoDAMapper& twodaMapper )
{
    TwoDAFileReader racialtypes_2da( twodaMapper.getFile( "racialtypes" ).c_str() );
    TwoDAFileReader racialsubtypes_2da( twodaMapper.getFile( "racialsubtypes" ).c_str() );

    for( size_t row = 0 ; row < racialsubtypes_2da.GetRowCount(); ++row ) {
        int nameRef, isPlayerRace;
        if( racialsubtypes_2da.Get2DAInt( "Name", row, nameRef )
                && racialsubtypes_2da.Get2DAInt( "PlayerRace", row, isPlayerRace )
                && isPlayerRace ) {
            std::string name;
            const auto nameOk = tlkSw.GetTalkString( nameRef, name );
            assert( nameOk );
            if( name.empty() ) {
                std::cerr << "importRaces: skipping row " << row << ", name is empty" << std::endl;
                continue;
            }

            int baseRaceRow;
            const auto baseRaceOk = racialsubtypes_2da.Get2DAInt( "BaseRace", row, baseRaceRow );
            assert( baseRaceOk );
            int baseRaceRef;
            const auto baseRaceIntOk = racialtypes_2da.Get2DAInt( "Name", baseRaceRow, baseRaceRef );
            assert( baseRaceIntOk );
            std::string baseRaceStr;
            const auto baseRaceStrOk = tlkSw.GetTalkString( baseRaceRef, baseRaceStr );
            assert( baseRaceStrOk );

            int descrRef;
            std::string descr;
            const auto descrOk = racialsubtypes_2da.Get2DAInt( "Description", row, descrRef );
            assert( descrOk );
            const auto descrRefOk = tlkSw.GetTalkString( descrRef, descr );
            assert( descrRefOk );

            std::unique_ptr< Race > race = std::make_unique< Race >( row, name, baseRaceStr );
            race->setDescription( translateToNwn2Tags( descr ) );

            const auto readAblMod = [ &racialsubtypes_2da, &race, row ]( AblScore abl, const char* colName ) {
                int ablMod = 0;
                const auto ablModOk = racialsubtypes_2da.Get2DAInt( colName, row, ablMod );
                if( !ablModOk ) {
                    ablMod = 0;
                }
                race->getAblAdjusts().setAbl( abl, ablMod );
                return true;
            };

            if( !( readAblMod( AblScore::Str, "StrAdjust" )
                   && readAblMod( AblScore::Dex, "DexAdjust" )
                   && readAblMod( AblScore::Con, "ConAdjust" )
                   && readAblMod( AblScore::Int, "IntAdjust" )
                   && readAblMod( AblScore::Wis, "WisAdjust" )
                   && readAblMod( AblScore::Cha, "ChaAdjust" ) ) ) {
                std::cerr << "importRaces: skipping " << name << ", could not read ability mod" << std::endl;
                continue;
            }

            std::string featsStr;
            const auto featsOk = racialsubtypes_2da.Get2DAString( "FeatsTable", row, featsStr );
            if( featsOk ) {
                boost::algorithm::to_lower( featsStr );
                auto feats = loadRacialFeatsTable( featsStr, twodaMapper );
                race->setFeats( std::move( feats ) );
            }

            std::cout << "importing race " << name << std::endl;
            nwnRules.setRace( std::move( race ) );
        }
    }
}

bool addGreatAblEffects( Feat& feat, const int id )
{
    const int greatCHA = 764;
    const int greatCON = 774;
    const int greatDEX = 784;
    const int greatINT = 794;
    const int greatWIS = 804;
    const int greatSTR = 814;

    const auto checkGreatAbl = [ &feat ]( int id, int ablId, FeatEffectType effect ) {
        if( id >= ablId && id < ablId + 10 ) {
            feat.addEffect( effect, 1 );
            return true;
        }
        return false;
    };

    return checkGreatAbl( id, greatSTR, FeatEffectType::StrBonus )
            || checkGreatAbl( id, greatDEX, FeatEffectType::DexBonus )
            || checkGreatAbl( id, greatCON, FeatEffectType::ConBonus )
            || checkGreatAbl( id, greatINT, FeatEffectType::IntBonus )
            || checkGreatAbl( id, greatWIS, FeatEffectType::WisBonus )
            || checkGreatAbl( id, greatCHA, FeatEffectType::ChaBonus );
}

bool addLuckOfHeroesEffects( Feat& feat, const int id )
{
    if( id == 382 ) {
        feat.addEffect( FeatEffectType::FortSave, 1 );
        feat.addEffect( FeatEffectType::RefSave, 1 );
        feat.addEffect( FeatEffectType::WillSave, 1 );
        return true;
    }
    return false;
}

bool addBasicSaveBonusEffects( Feat& feat, const int id )
{
    if( id == 14 ) {
        feat.addEffect( FeatEffectType::FortSave, 2 );
        return true;
    }
    if( id == 24 ) {
        feat.addEffect( FeatEffectType::RefSave, 2 );
        return true;
    }
    if( id == 22 ) {
        feat.addEffect( FeatEffectType::WillSave, 2 );
        return true;
    }
    return false;
}

void addFeatEffects( Feat& feat )
{
    const auto id = feat.getId();

    addGreatAblEffects( feat, id )
            || addLuckOfHeroesEffects( feat, id )
            || addBasicSaveBonusEffects( feat, id );
}

void importFeats( Rules &nwnRules, const TlkSwitcher& tlkSw, TwoDAMapper& twodaMapper )
{
    TwoDAFileReader feat_2da( twodaMapper.getFile( "feat" ).c_str() );

    for( size_t row = 0 ; row < feat_2da.GetRowCount(); ++row ) {
        int featRef;
        if( feat_2da.Get2DAInt( "FEAT", row, featRef )) {
            std::string name;
            const auto nameOk = tlkSw.GetTalkString( featRef, name );
            if( !nameOk || name.empty() ) {
                std::cerr << "importFeats: skipping row " << row << std::endl;
                continue;
            }

            int descrRef;
            std::string descr;
            const auto descrOk = feat_2da.Get2DAInt( "DESCRIPTION", row, descrRef );
            if( !descrOk ) {
                std::cerr << "importFeats: skipping row " << row << ", could not read description ref" << std::endl;
                continue;
            }
            const auto descrRefOk = tlkSw.GetTalkString( descrRef, descr );
            if( !descrRefOk ) {
                std::cerr << "importFeats: skipping row " << row << ", could not read description" << std::endl;
                continue;
            }

            int removed;
            const auto removedOk = feat_2da.Get2DAInt( "REMOVED", row, removed );
            if( !removedOk ) {
                std::cerr << "importFeats: skipping row " << row << ", could not read REMOVED" << std::endl;
                continue;
            }
            if( removed ) {
                std::cout << "importFeats: skipping row " << row << ", REMOVED=1" << std::endl;
                continue;
            }

            std::unique_ptr< Feat > feat = std::make_unique< Feat >( row, name );
            feat->setDescription( translateToNwn2Tags( descr ) );

            const auto readColumn = [ &feat_2da, row, &feat ]( const char* col ) {
                int val;
                const auto valOk = feat_2da.Get2DAInt( col, row, val );
                if( valOk ) {
                    feat->setColumn( col, val );
                }
            };

            const std::vector<const char*> featCols = { // LABEL FEAT DESCRIPTION ICON
                "MINATTACKBONUS",
                "MINSTR",
                "MINDEX",
                "MININT",
                "MINWIS",
                "MINCON",
                "MINCHA",
                "MAXSTR",
                "MAXDEX",
                "MAXINT",
                "MAXWIS",
                "MAXCON",
                "MAXCHA",
                "MINSPELLLVL",
                "MINCASTERLVL",
                "PREREQFEAT1",
                "PREREQFEAT2",
                "GAINMULTIPLE",
                "EFFECTSSTACK",
                "ALLCLASSESCANUSE",
                "CATEGORY",
                "MAXCR",
                "SPELLID",
                "SUCCESSOR",
                "CRValue",
                "USESPERDAY",
                "USESMAPFEAT",
                "MASTERFEAT",
                "TARGETSELF",
                "OrReqFeat0",
                "OrReqFeat1",
                "OrReqFeat2",
                "OrReqFeat3",
                "OrReqFeat4",
                "OrReqFeat5",
                "REQSKILL",
                "ReqSkillMaxRanks",
                "ReqSkillMinRanks",
                "REQSKILL2",
                "ReqSkillMaxRanks2",
                "ReqSkillMinRanks2",
                // "Constant",
                "TOOLSCATEGORIES",
                "HostileFeat",
                "MinLevel",
                "MinLevelClass",
                "MaxLevel",
                "MinFortSave",
                "PreReqEpic",
                // "FeatCategory",
                "IsActive",
                "IsPersistent",
                "ToggleMode",
                "Cooldown",
                "DMFeat",
                // "REMOVED",
                "AlignRestrict",
                "ImmunityType",
                "Instant"
            };
            for( const auto& col : featCols ) {
                readColumn( col );
            }

            addFeatEffects( *feat );

            std::cout << "importing feat ID " << row << ": " << name << std::endl;
            nwnRules.setFeat( std::move( feat ) );
        }
    }
}

std::string getCurrentTimeString()
{
    const auto now = time(0);
    std::string currentTime{ ctime( &now ) };
    boost::trim_right( currentTime );
    return currentTime;
}

} // namespace

void readBasegameRules( const std::string& outputPath, const std::string& nwn2Path, const TlkFileReader16& dialogTlk )
{
    TwoDAMapper twodaMapper( nwn2Path, outputPath );
    TlkSwitcher tlkSw( dialogTlk, nullptr );

    Rules nwnRules;

    importClasses( nwnRules, tlkSw, twodaMapper );
    importRaces( nwnRules, tlkSw, twodaMapper );
    importFeats( nwnRules, tlkSw, twodaMapper );

    nwnRules.setDescription( "NWN2 base game" );
    nwnRules.save( ( outputPath + "\\nwn2.xml" ).c_str() );
}

void readScodRules( const std::string& outputPath, const std::string& nwn2Path, const TlkFileReader16& dialogTlk )
{
    TwoDAMapper twodaMapper( nwn2Path, outputPath );

    twodaMapper.readHak( "C:\\Users\\Raihan\\Documents\\Neverwinter Nights 2\\hak\\scod3_2da_rc.hak" );
    twodaMapper.readHak( "C:\\Users\\Raihan\\Documents\\Neverwinter Nights 2\\hak\\scod3_2da_main.hak" );
    twodaMapper.readHak( "C:\\Users\\Raihan\\Documents\\Neverwinter Nights 2\\hak\\scod3_2da_core.hak" );
    TlkFileReader16 otherTlk( "C:\\Users\\Raihan\\Documents\\Neverwinter Nights 2\\tlk\\scod2.tlk" );

    TlkSwitcher tlkSw( dialogTlk, &otherTlk );

    Rules nwnRules;

    importClasses( nwnRules, tlkSw, twodaMapper );
    importRaces( nwnRules, tlkSw, twodaMapper );
    importFeats( nwnRules, tlkSw, twodaMapper );

    nwnRules.setDescription( std::string( "Sigil City of Doors, " ) + getCurrentTimeString() );
    nwnRules.save( ( outputPath + "\\scod.xml" ).c_str() );
}

int main()
{
    const auto outputPath = std::string( OUTPUT_PATH );
    const auto nwn2Path = std::string( NWN2_PATH );
    TlkFileReader16 dialogTlk( ( nwn2Path + "\\dialog.TLK" ).c_str() );

    readBasegameRules( outputPath, nwn2Path, dialogTlk );
    readScodRules( outputPath, nwn2Path, dialogTlk );
}
