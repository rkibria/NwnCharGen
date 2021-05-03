#include "nwnchargen.h"

#ifdef ASM_CRASH_REPORT
#include <QMessageBox>
#include <asmCrashReport/asmCrashReport.h>
#endif

#include <QApplication>
#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName( QStringLiteral( "NwnCharGen" ) );
    a.setApplicationVersion( QStringLiteral( NWNCHARGEN_VERSION ) );

    #ifdef ASM_CRASH_REPORT
      asmCrashReport::setSignalHandler( QString(), [] (const QString &inFileName, bool inSuccess) {
          QString  message;

          if ( inSuccess ) {
             message = QStringLiteral( "Sorry, %1 has crashed. A log file was written to:\n\n%2\n" ).arg( QCoreApplication::applicationName(), inFileName );
          }
          else {
             message = QStringLiteral( "Sorry, %1 has crashed and we could not write a log file to:\n\n%2\n" ).arg( QCoreApplication::applicationName(), inFileName );
          }

          QMessageBox::critical( nullptr, QObject::tr( "%1 Crashed" ).arg( QCoreApplication::applicationName() ), message );
      });
    #endif

    QFile f( ":qdarkstyle/style.qss" );
    if ( !f.exists() ) {
        printf( "Unable to set stylesheet, file not found\n" );
    }
    else {
        f.open( QFile::ReadOnly | QFile::Text );
        QTextStream ts( &f );
        qApp->setStyleSheet( ts.readAll() );
    }

    NwnCharGen w;
    w.show();
    return a.exec();
}
