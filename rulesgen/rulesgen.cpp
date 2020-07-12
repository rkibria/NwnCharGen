#include <iostream>

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

using TlkFileReader16 = TlkFileReader<NWN::ResRef16>;

void importClasses( Rules &nwnRules )
{
    TlkFileReader16 dialog_tlk( NWN2_PATH "\\dialog.TLK" );
    TwoDAFileReader classes_2da( EXTRACT_PATH "\\2DA_X2\\classes.2da" );

    for( size_t i = 0 ; i < classes_2da.GetRowCount(); ++i ) {
        int nameRef, isPlayerClass;
        if( classes_2da.Get2DAInt( "Name", i, nameRef )
                && classes_2da.Get2DAInt( "PlayerClass", i, isPlayerClass )
                && isPlayerClass ) {
            std::string name;
            const auto nameOk = dialog_tlk.GetTalkString( nameRef, name );
            assert( nameOk );

            int descrRef;
            std::string descr;
            const auto descrOk = classes_2da.Get2DAInt( "Description", i, descrRef );
            assert( descrOk );
            const auto descrRefOk = dialog_tlk.GetTalkString( descrRef, descr );
            assert( descrRefOk );

            int hitDieInt;
            const auto hitdieOk = classes_2da.Get2DAInt( "HitDie", i, hitDieInt );
            assert( hitdieOk );
            const auto hitDie = intToDice( hitDieInt );

            std::unique_ptr< ChClass > chClass = std::make_unique< ChClass >( name );
            chClass->setDescription( descr );
            chClass->setHitDie( hitDie );
            nwnRules.setChClass( std::move( chClass ) );
        }
    }
}

int main()
{
    Rules nwnRules;

    importClasses( nwnRules );

    nwnRules.save( OUTPUT_PATH "\\nwn2.xml" );
}
