#include <iostream>
#include <unordered_map>
#include <stdexcept>
#include <sstream>
#include <set>
#include <map>

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
    TlkSwitcher( TlkFileReader16& main, TlkFileReader16* aux )
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

std::unique_ptr< FeatsPerLevelMap > loadClassFeatsTable( const std::string& tableName, TwoDAMapper& twodaMapper )
{
    auto feats = std::make_unique< FeatsPerLevelMap >();

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

        constexpr const int kAutomaticallyGranted = 3;
        if( list == kAutomaticallyGranted ) {
            const auto grantedLvl = granted - 1;
            if( (*feats).find( grantedLvl ) == (*feats).end() ) {
                (*feats)[ grantedLvl ] = std::set<int>();
            }
            (*feats)[ grantedLvl ].insert( feat );
        }
    }

    return feats;
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
            if( featsOk ) {
                boost::algorithm::to_lower( featsStr );
                featsPerLvl = loadClassFeatsTable( featsStr, twodaMapper );
            }

            std::string bonusfeatsStr;
            const auto bonusfeatsOk = classes_2da.Get2DAString( "BonusFeatsTable", row, bonusfeatsStr );
            auto bonusFeats = std::make_unique< std::vector< bool > >();
            if( bonusfeatsOk ) {
                boost::algorithm::to_lower( bonusfeatsStr );
                bonusFeats = loadBonusFeatsTable( bonusfeatsStr, twodaMapper );
            }

            std::cout << "importing class " << name << std::endl;
            std::unique_ptr< ChClass > chClass = std::make_unique< ChClass >( name );
            chClass->setDescription( translateToNwn2Tags( descr ) );
            chClass->setHitDie( hitDie );
            chClass->setBabProgression( prg );
            chClass->setSaves( saves );
            chClass->setFeatsPerLvl( std::move( featsPerLvl ) );
            chClass->setBonusFeats( std::move( bonusFeats ) );
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

            std::unique_ptr< Race > race = std::make_unique< Race >( name, baseRaceStr );
            race->setDescription( translateToNwn2Tags( descr ) );

            const auto readAblMod = [ &racialsubtypes_2da, &race, row ]( AblScore abl, const char* colName ) {
                int ablMod;
                const auto ablModOk = racialsubtypes_2da.Get2DAInt( colName, row, ablMod );
                if( !ablModOk ) {
                    return false;
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

            int allClassesCanUse;
            const auto allClassesCanUseOk = feat_2da.Get2DAInt( "ALLCLASSESCANUSE", row, allClassesCanUse );
            if( !allClassesCanUseOk ) {
                std::cerr << "importFeats: skipping row " << row << ", could not read ALLCLASSESCANUSE" << std::endl;
                continue;
            }

            std::unique_ptr< Feat > feat = std::make_unique< Feat >( row, name );
            feat->setDescription( translateToNwn2Tags( descr ) );
            feat->setAllClassesCanUse( allClassesCanUse ? true : false );

            std::cout << "importing feat ID " << row << ": " << name << std::endl;
            nwnRules.setFeat( std::move( feat ) );
        }
    }
}

} // namespace

int main()
{
    const auto outputPath = std::string( OUTPUT_PATH );
    const auto nwn2Path = std::string( NWN2_PATH );

    TwoDAMapper twodaMapper( nwn2Path, outputPath );

    twodaMapper.readHak( "C:\\Users\\Raihan\\Documents\\Neverwinter Nights 2\\hak\\scod3_2da_rc.hak" );
    twodaMapper.readHak( "C:\\Users\\Raihan\\Documents\\Neverwinter Nights 2\\hak\\scod3_2da_core.hak" );
    twodaMapper.readHak( "C:\\Users\\Raihan\\Documents\\Neverwinter Nights 2\\hak\\scod3_2da_main.hak" );
    TlkFileReader16 otherTlk( "C:\\Users\\Raihan\\Documents\\Neverwinter Nights 2\\tlk\\scod2.tlk" );

    TlkFileReader16 dialogTlk( ( nwn2Path + "\\dialog.TLK" ).c_str() );

    TlkSwitcher tlkSw( dialogTlk, &otherTlk );

    Rules nwnRules;

    importClasses( nwnRules, tlkSw, twodaMapper );
    importRaces( nwnRules, tlkSw, twodaMapper );
    importFeats( nwnRules, tlkSw, twodaMapper );

    nwnRules.save( ( outputPath + "\\scod.xml" ).c_str() );
}
