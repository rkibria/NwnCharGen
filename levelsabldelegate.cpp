#include <QMouseEvent>

#include "levelsabldelegate.h"
#include "levelsmodel.h"
#include "nwnchargen.h"

#include <Nwn/character.hpp>

void LevelsAblDelegate::paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
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
