#include "levelsclassdelegate.h"

void LevelsClassDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    QPen pen( option.palette.highlight().color() );
    painter->setPen( pen );
    painter->drawRoundedRect( option.rect.adjusted( 3, 3, -3, -3 ), 5.0, 5.0 );
    painter->restore();

    const auto chclass = qvariant_cast<QString>( index.data() );
    painter->drawText( option.rect, Qt::AlignCenter, chclass );
}
