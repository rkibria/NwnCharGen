#include <QMouseEvent>

#include "featchoicedelegate.h"
#include "levelsmodel.h"
#include "nwnchargen.h"
#include "chclassdialog.h"

#include <Nwn/character.hpp>

void FeatChoiceDelegate::paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    const auto row = index.row();
    painter->drawText( option.rect, Qt::AlignCenter, row%2==0 ? "++++" : "--" );
}

QSize FeatChoiceDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const auto row = index.row();
    return QSize(100, 50 + (row%2)*300 );
}
