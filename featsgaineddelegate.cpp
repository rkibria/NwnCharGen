#include <QMouseEvent>

#include "featsgaineddelegate.h"
#include "levelsmodel.h"
#include "nwnchargen.h"
#include "chclassdialog.h"

#include <Nwn/character.hpp>

void FeatsGainedDelegate::paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    painter->drawText( option.rect, Qt::AlignCenter, "++++" );
}
