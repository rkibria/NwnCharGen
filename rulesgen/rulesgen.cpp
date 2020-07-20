#include <iostream>
#include <unordered_map>
#include <stdexcept>
#include <sstream>

#include <Precomp.h>
#include <2DAFileReader.h>
#include <TlkFileReader.h>

#include <Nwn/base.hpp>
#include <Nwn/character.hpp>
#include <Nwn/ablblock.hpp>
#include <Nwn/race.hpp>
#include <Nwn/rules.hpp>
#include <Nwn/chclass.hpp>
using namespace Nwn;

#include <boost/algorithm/string.hpp>

#include "twodamapper.h"

using TlkFileReader16 = TlkFileReader<NWN::ResRef16>;

namespace  {

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
    static std::unordered_map< std::string, std::vector< SavingThrows > > savesTables;

    constexpr const auto colLevel = "Level";
    constexpr const auto colFort = "FortSave";
    constexpr const auto colRef = "RefSave";
    constexpr const auto colWill = "WillSave";
    const auto lowerName = boost::to_lower_copy( tableName );
    if( savesTables.count( lowerName ) == 0 ) {
        TwoDAFileReader cls_savthr_2da( twodaMapper.getFile( lowerName ).c_str() );
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

void importClasses( Rules &nwnRules, TlkFileReader16& dialog_tlk, TwoDAMapper& twodaMapper )
{
    TwoDAFileReader classes_2da( twodaMapper.getFile( "classes" ).c_str() );

    for( size_t row = 0 ; row < classes_2da.GetRowCount(); ++row ) {
        int nameRef, isPlayerClass;
        if( classes_2da.Get2DAInt( "Name", row, nameRef )
                && classes_2da.Get2DAInt( "PlayerClass", row, isPlayerClass )
                && isPlayerClass ) {
            std::string name;
            const auto nameOk = dialog_tlk.GetTalkString( nameRef, name );
            assert( nameOk );
            std::cout << "importing class " << name << std::endl;

            int descrRef;
            std::string descr;
            const auto descrOk = classes_2da.Get2DAInt( "Description", row, descrRef );
            assert( descrOk );
            const auto descrRefOk = dialog_tlk.GetTalkString( descrRef, descr );
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

            std::unique_ptr< ChClass > chClass = std::make_unique< ChClass >( name );
            chClass->setDescription( translateToNwn2Tags( descr ) );
            chClass->setHitDie( hitDie );
            chClass->setBabProgression( prg );
            chClass->setSaves( saves );
            nwnRules.setChClass( std::move( chClass ) );
        }
    }
}

void importRaces( Rules &nwnRules, TlkFileReader16& dialog_tlk, TwoDAMapper& twodaMapper )
{
    TwoDAFileReader racialtypes_2da( twodaMapper.getFile( "racialtypes" ).c_str() );
    TwoDAFileReader racialsubtypes_2da( twodaMapper.getFile( "racialsubtypes" ).c_str() );

    for( size_t row = 0 ; row < racialsubtypes_2da.GetRowCount(); ++row ) {
        int nameRef, isPlayerRace;
        if( racialsubtypes_2da.Get2DAInt( "Name", row, nameRef )
                && racialsubtypes_2da.Get2DAInt( "PlayerRace", row, isPlayerRace )
                && isPlayerRace ) {
            std::string name;
            const auto nameOk = dialog_tlk.GetTalkString( nameRef, name );
            assert( nameOk );
            std::cout << "importing race " << name << std::endl;

            int baseRaceRow;
            const auto baseRaceOk = racialsubtypes_2da.Get2DAInt( "BaseRace", row, baseRaceRow );
            assert( baseRaceOk );
            int baseRaceRef;
            const auto baseRaceIntOk = racialtypes_2da.Get2DAInt( "Name", baseRaceRow, baseRaceRef );
            assert( baseRaceIntOk );
            std::string baseRaceStr;
            const auto baseRaceStrOk = dialog_tlk.GetTalkString( baseRaceRef, baseRaceStr );
            assert( baseRaceStrOk );

            int descrRef;
            std::string descr;
            const auto descrOk = racialsubtypes_2da.Get2DAInt( "Description", row, descrRef );
            assert( descrOk );
            const auto descrRefOk = dialog_tlk.GetTalkString( descrRef, descr );
            assert( descrRefOk );

            std::unique_ptr< Race > race = std::make_unique< Race >( name, baseRaceStr );
            race->setDescription( translateToNwn2Tags( descr ) );

            const auto readAblMod = [ &racialsubtypes_2da, &race, row ]( AblScore abl, const char* colName ) {
                int ablMod;
                const auto ablModOk = racialsubtypes_2da.Get2DAInt( colName, row, ablMod );
                assert( ablModOk );
                race->getAblAdjusts().setAbl( abl, ablMod );
            };

            readAblMod( AblScore::Str, "StrAdjust" );
            readAblMod( AblScore::Dex, "DexAdjust" );
            readAblMod( AblScore::Con, "ConAdjust" );
            readAblMod( AblScore::Int, "IntAdjust" );
            readAblMod( AblScore::Wis, "WisAdjust" );
            readAblMod( AblScore::Cha, "ChaAdjust" );

            nwnRules.setRace( std::move( race ) );
        }
    }
}

} // namespace

int main()
{
    const auto outputPath = std::string( OUTPUT_PATH );
    const auto nwn2Path = std::string( NWN2_PATH );

    TwoDAMapper twodaMapper( nwn2Path, outputPath );
    TlkFileReader16 dialog_tlk( ( nwn2Path + "\\dialog.TLK" ).c_str() );

    Rules nwnRules;

    importClasses( nwnRules, dialog_tlk, twodaMapper );
    importRaces( nwnRules, dialog_tlk, twodaMapper );

    nwnRules.save( ( outputPath + "\\nwn2.xml" ).c_str() );
}
