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

#include "unziphelper.h"

#include <boost/algorithm/string.hpp>

using TlkFileReader16 = TlkFileReader<NWN::ResRef16>;

static const std::unordered_map<std::string, BabProgression> map_2da_bab {
    { "CLS_ATK_3", BabProgression::low },
    { "CLS_ATK_2", BabProgression::medium },
    { "CLS_ATK_1", BabProgression::high }
};

/*
void loadSavesTable( const std::string& tableName )
{
    std::string tablePath = EXTRACT_PATH "\\2DA\\";
    tablePath += tableName;
    tablePath += ".2da";
    TwoDAFileReader cls_savthr_2da( tablePath );
    std::cout << tablePath << std::endl;
}

void importClasses( Rules &nwnRules )
{
    TlkFileReader16 dialog_tlk( NWN2_PATH "\\dialog.TLK" );
    TwoDAFileReader classes_2da( EXTRACT_PATH "\\2DA_X2\\classes.2da" );

    for( size_t row = 0 ; row < classes_2da.GetRowCount(); ++row ) {
        int nameRef, isPlayerClass;
        if( classes_2da.Get2DAInt( "Name", row, nameRef )
                && classes_2da.Get2DAInt( "PlayerClass", row, isPlayerClass )
                && isPlayerClass ) {
            std::string name;
            const auto nameOk = dialog_tlk.GetTalkString( nameRef, name );
            assert( nameOk );

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
            assert( map_2da_bab.count( attackStr ) == 1 );
            const auto babPrg = map_2da_bab.at( attackStr );

            std::string savesStr;
            const auto savesOk = classes_2da.Get2DAString( "SavingThrowTable", row, savesStr );
            assert( savesOk );
            boost::algorithm::to_lower( savesStr );
            loadSavesTable( savesStr );

            std::unique_ptr< ChClass > chClass = std::make_unique< ChClass >( name );
            chClass->setDescription( descr );
            chClass->setHitDie( hitDie );
            chClass->setBabProgression( babPrg );
            nwnRules.setChClass( std::move( chClass ) );
        }
    }
}
*/

int main()
{
    Rules nwnRules;

//    importClasses( nwnRules );

    UnzipHelper::extract( NWN2_PATH "\\Data\\2DA.zip", "2DA/actions.2da" );

    nwnRules.save( OUTPUT_PATH "\\nwn2.xml" );
}