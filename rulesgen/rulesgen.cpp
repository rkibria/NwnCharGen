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

void loadSavesTable( const std::string& tableName, TwoDAMapper& twodaMapper )
{
    TwoDAFileReader cls_savthr_2da( twodaMapper.getFile( tableName ).c_str() );
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
        std::vector<int> prg( Character::maxLevel );
        for( size_t row = 0 ; row < Character::maxLevel; ++row ) {
            int bab;
            const auto babOk = attack_2da.Get2DAInt( colName, row, bab );
            assert( babOk );
            prg[ row ] = bab;
        }
        attackTables[ lowerName ] = prg;
    }
    assert( attackTables.count( lowerName ) == 1 );
    return attackTables[ lowerName ];
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
            loadSavesTable( savesStr, twodaMapper );

            std::unique_ptr< ChClass > chClass = std::make_unique< ChClass >( name );
            chClass->setDescription( descr );
            chClass->setHitDie( hitDie );
            chClass->setBabProgression( prg );
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
