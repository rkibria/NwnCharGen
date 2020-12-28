#include <QMouseEvent>
#include <QRect>

#include "featchoicedelegate.h"
#include "levelsmodel.h"
#include "nwnchargen.h"

#include <Nwn/rules.hpp>
#include <Nwn/character.hpp>

namespace {
constexpr int heightPerFeatBox = 25;
}

void FeatChoiceDelegate::paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    const auto lvl = index.row();

    const auto levelsModel = static_cast<const LevelsModel*>( index.model() );
    const auto nwnCharGen = levelsModel->getNwnCharGen();
    const auto nwnRules = nwnCharGen->getRules();
    const auto nwnChar = nwnCharGen->getCharacter();

    const auto numFeatChoices = nwnRules->getNumFeatChoicesAtLvl( nwnChar, lvl );

    if( numFeatChoices ) {
        painter->save();
        painter->setRenderHint(QPainter::Qt4CompatiblePainting, true);
        QPen pen( option.palette.highlight().color() );
        painter->setPen( pen );
        QRect rect = option.rect;
        rect.setHeight(heightPerFeatBox);
        for( int i = 0; i < numFeatChoices; ++i ) {
            painter->drawRoundedRect( rect.adjusted( 3, 3, -3, -3 ), 5.0, 5.0 );
            painter->drawText( rect, Qt::AlignCenter, "Select Feat" );
            rect.moveTop( rect.top() + heightPerFeatBox );
        }
        painter->restore();
    }
}

QSize FeatChoiceDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const auto lvl = index.row();

    const auto levelsModel = static_cast<const LevelsModel*>( index.model() );
    const auto nwnCharGen = levelsModel->getNwnCharGen();
    const auto nwnRules = nwnCharGen->getRules();
    const auto nwnChar = nwnCharGen->getCharacter();

    const auto numFeatChoices = nwnRules->getNumFeatChoicesAtLvl( nwnChar, lvl );

    return QSize(100, numFeatChoices * heightPerFeatBox );
}

#include <iostream>
bool FeatChoiceDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                                     const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if ( event->type() != QEvent::MouseButtonPress ) {
        return true;
    }

    const auto mouseEvent = static_cast<QMouseEvent*>( event );
    if( ! ( mouseEvent->buttons() & Qt::LeftButton ) ) {
        return true;
    }

    const auto lvl = index.row();

    const auto levelsModel = static_cast<const LevelsModel*>( index.model() );
    const auto nwnCharGen = levelsModel->getNwnCharGen();
    const auto nwnRules = nwnCharGen->getRules();
    const auto nwnChar = nwnCharGen->getCharacter();

    const auto numFeatChoices = nwnRules->getNumFeatChoicesAtLvl( nwnChar, lvl );
    if( !numFeatChoices ) {
        return true;
    }

    const auto pos = mouseEvent->pos();
    const auto y = pos.y() - option.rect.y();
    const auto selectedIndex = static_cast<int>(y / heightPerFeatBox);
    if( selectedIndex >= numFeatChoices ) {
        return true;
    }

    std::cout << selectedIndex << "\n";

    return true;
}
