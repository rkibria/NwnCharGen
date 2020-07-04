#include <QMouseEvent>

#include "levelsabldelegate.h"
#include "levelsmodel.h"
#include "nwnchargen.h"

#include <Nwn/character.hpp>

using namespace Nwn;

namespace {

AblScore getColumnAbl( const QModelIndex &index )
{
    switch( index.column() ) {
    case LevelsModel::kSTRCol: return AblScore::Str;
    case LevelsModel::kDEXCol: return AblScore::Dex;
    case LevelsModel::kCONCol: return AblScore::Con;
    case LevelsModel::kINTCol: return AblScore::Int;
    case LevelsModel::kWISCol: return AblScore::Wis;
    case LevelsModel::kCHACol: return AblScore::Cha;
    default: return AblScore::Str;
    }
}

} // namespace

void LevelsAblDelegate::paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    const auto lvl = index.row() + 1;
    if( lvl > 1 && lvl % 4 == 0 ) {
        const auto levelsModel = static_cast<const LevelsModel*>( index.model() );
        const auto nwnCharGen = levelsModel->getNwnCharGen();
        const auto nwnChar = nwnCharGen->getCharacter();
        const auto curIncAbl = nwnChar->getAblInc( lvl / 4 - 1 );
        const auto curAblCol = getColumnAbl( index );

        if( curIncAbl == curAblCol ) {
            painter->save();
            painter->setRenderHint( QPainter::Qt4CompatiblePainting, true );
            QPen pen( option.palette.highlight().color() );
            painter->setPen( pen );
            painter->drawRoundedRect( option.rect.adjusted( 3, 3, -3, -3 ), 8.0, 8.0 );
            painter->restore();
        }
    }

    const auto ablValue = qvariant_cast<QString>( index.data() );
    painter->drawText( option.rect, Qt::AlignCenter, ablValue );
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
