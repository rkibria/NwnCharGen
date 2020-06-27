#include <QMouseEvent>

#include "levelsclassdelegate.h"
#include "levelsmodel.h"
#include "nwnchargen.h"

#include <Nwn/character.hpp>

void LevelsClassDelegate::paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    painter->save();
    QPen pen( option.palette.highlight().color() );
    painter->setPen( pen );
    painter->drawRoundedRect( option.rect.adjusted( 3, 3, -3, -3 ), 5.0, 5.0 );
    painter->restore();

    const auto chclass = qvariant_cast<QString>( index.data() );
    painter->drawText( option.rect, Qt::AlignCenter, chclass );
}

bool LevelsClassDelegate::editorEvent(QEvent *event,
                                      QAbstractItemModel *model,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index)
{
    Q_UNUSED( option );

    if ( event->type() != QEvent::MouseButtonPress ) {
        return false;
    }

    const auto mouseEvent = static_cast<QMouseEvent*>( event );
    if( ! ( mouseEvent->buttons() & Qt::LeftButton ) ) {
        return false;
    }

    const auto lvl = index.row();
    const auto levelsModel = static_cast<LevelsModel*>( model );
    const auto nwnCharGen = levelsModel->getNwnCharGen();
    nwnCharGen->getCharacter()->setLevel( lvl, "Changed" );
    nwnCharGen->updateAll();

    return true;
}
