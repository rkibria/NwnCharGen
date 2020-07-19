#include <iostream>
#include <unordered_map>
#include <stdexcept>

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

const SavesArray& loadSavesTable( const std::string& tableName, TwoDAMapper& twodaMapper )
{
    static std::unordered_map< std::string, SavesArray > savesTables;

    constexpr const auto colLevel = "Level";
    constexpr const auto colFort = "FortSave";
    constexpr const auto colRef = "RefSave";
    constexpr const auto colWill = "WillSave";
    const auto lowerName = boost::to_lower_copy( tableName );
    if( savesTables.count( lowerName ) == 0 ) {
        TwoDAFileReader cls_savthr_2da( twodaMapper.getFile( lowerName ).c_str() );
        const auto nRows = cls_savthr_2da.GetRowCount();
        savesTables.emplace( lowerName,
                             SavesArray{ std::vector<int>(nRows), std::vector<int>(nRows), std::vector<int>(nRows) } );
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
            saves[ static_cast< int >( SavingThrow::Fort ) ][ lvl ] = fort;
            saves[ static_cast< int >( SavingThrow::Ref ) ][ lvl ] = ref;
            saves[ static_cast< int >( SavingThrow::Will ) ][ lvl ] = will;
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
            chClass->setDescription( descr );
            chClass->setHitDie( hitDie );
            chClass->setBabProgression( prg );
            chClass->setSaves( saves );
            nwnRules.setChClass( std::move( chClass ) );
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

    nwnRules.save( ( outputPath + "\\nwn2.xml" ).c_str() );
}
