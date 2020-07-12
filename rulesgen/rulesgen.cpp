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

int main()
{
    Rules nwnRules;

    using TlkFileReader16 = TlkFileReader<NWN::ResRef16>;
    TlkFileReader16 dialog_tlk( NWN2_PATH "\\dialog.TLK" );

    TwoDAFileReader classes_2da( EXTRACT_PATH "\\2DA_X2\\classes.2da" );

    std::string name, descr;
    int nameRef, descrRef, isPlayerClass;
    for( size_t i = 0 ; i < classes_2da.GetRowCount(); ++i ) {
        if( classes_2da.Get2DAInt( "Name", i, nameRef )
                && classes_2da.Get2DAInt( "PlayerClass", i, isPlayerClass )
                && isPlayerClass ) {
            const auto nameOk = dialog_tlk.GetTalkString( nameRef, name );
            assert( nameOk );

            const auto descrOk = classes_2da.Get2DAInt( "Description", i, descrRef );
            assert( descrOk );
            const auto descrRefOk = dialog_tlk.GetTalkString( descrRef, descr );
            assert( descrRefOk );

            std::unique_ptr< ChClass > chClass = std::make_unique< ChClass >( name );
            chClass->setDescription( descr );
            nwnRules.setChClass( std::move( chClass ) );
        }
    }

    nwnRules.save( OUTPUT_PATH "\\nwn2.xml" );
}
