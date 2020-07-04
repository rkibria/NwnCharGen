#include <QMouseEvent>

#include "levelsabldelegate.h"
#include "levelsmodel.h"
#include "nwnchargen.h"

#include <Nwn/character.hpp>

void LevelsAblDelegate::paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    const auto lvl = index.row() + 1;
    if( lvl > 1 && lvl % 4 == 0 ) {
        painter->save();
        painter->setRenderHint(QPainter::Qt4CompatiblePainting, true);
        QPen pen( option.palette.highlight().color() );
        painter->setPen( pen );
        painter->drawRoundedRect( option.rect.adjusted( 3, 3, -3, -3 ), 8.0, 8.0 );
        painter->restore();
    }

    const auto chclass = qvariant_cast<QString>( index.data() );
    painter->drawText( option.rect, Qt::AlignCenter, chclass );
}

bool LevelsAblDelegate::editorEvent(QEvent *event,
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

    return true;
}
