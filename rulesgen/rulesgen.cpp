#include <iostream>

#include <Precomp.h>
#include <2DAFileReader.h>
#include <TlkFileReader.h>

#include <Nwn/rules.hpp>

int main()
{
    TwoDAFileReader classes_2da( EXTRACT_PATH "\\2DA_X2\\classes.2da" );

    std::cout << "row count: " << classes_2da.GetRowCount() << std::endl;
    std::cout << "column count: " << classes_2da.GetColumnCount() << std::endl;

    const auto labelColName = "Label";
    const auto hasCol = classes_2da.HasColumn( labelColName );
    std::cout << "has column '" << labelColName << "': " << hasCol << std::endl;

    if( !hasCol ) {
        exit(1);
    }

    const auto nameColName = "Name";
    std::string strVal;
    int intVal;
    for( size_t i = 0 ; i < classes_2da.GetRowCount(); ++i ) {
        if( classes_2da.Get2DAString( labelColName, i, strVal ) ) {
            if( classes_2da.Get2DAInt( nameColName, i, intVal ) ) {
                std::cout << i << " " << strVal << " " << intVal << std::endl;
            }
        }
    }

    using TlkFileReader16 = TlkFileReader<NWN::ResRef16>;

    TlkFileReader16 dialog_tlk( NWN2_PATH "\\dialog.TLK" );

    for( TlkFileReader16::StrRef ref = 0; ref < 10; ++ref ) {
        if( dialog_tlk.GetTalkString( ref, strVal ) ) {
            std::cout << ref << ": " << strVal << std::endl;
        }
    }

}
