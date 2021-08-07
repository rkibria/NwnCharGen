#include <QMouseEvent>
#include <QRect>

#include "featchoicedelegate.h"
#include "levelsmodel.h"
#include "nwnchargen.h"
#include "featdialog.h"

#include <Nwn/rules.hpp>
#include <Nwn/character.hpp>
#include <Nwn/feat.hpp>

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

    const auto numFeatChoices = nwnRules->getNumTotalFeatChoicesAtLvl( nwnChar, lvl );

    if( numFeatChoices ) {
        const auto& choices = nwnChar->getFeatChoicesAtLvl( lvl );

        painter->save();
        painter->setRenderHint(QPainter::Qt4CompatiblePainting, true);
        QPen pen( option.palette.highlight().color() );
        painter->setPen( pen );
        QPen textPen( option.palette.text().color() );
        QRect rect = option.rect;
        rect.setHeight(heightPerFeatBox);
        static const std::string unassignedText = "Select Feat";
        for( int i = 0; i < numFeatChoices; ++i ) {
            auto text = &unassignedText;

            const auto featId = ( i < choices.size() ) ? choices[ i ] : Nwn::INVALID_FEAT_ID;
            const auto feat = nwnRules->getFeat( featId );
            if( feat ) {
                text = &feat->getName();
            }

            painter->drawRoundedRect( rect.adjusted( 3, 3, -3, -3 ), 5.0, 5.0 );

            if( text != &unassignedText ) {
                painter->save();
                painter->setPen( textPen );
            }
            painter->drawText( rect, Qt::AlignCenter, text->c_str() );
            if( text != &unassignedText ) {
                painter->restore();
            }

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

    const auto numFeatChoices = nwnRules->getNumTotalFeatChoicesAtLvl( nwnChar, lvl );

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

    const auto numFeatChoices = nwnRules->getNumTotalFeatChoicesAtLvl( nwnChar, lvl );
    if( !numFeatChoices ) {
        return true;
    }

    const auto pos = mouseEvent->pos();
    const auto y = pos.y() - option.rect.y();
    const auto selectedIndex = static_cast<int>( y / heightPerFeatBox );
    if( selectedIndex >= numFeatChoices ) {
        return true;
    }

    FeatDialog ftd( nwnCharGen->getRules(), nwnCharGen->getCharacter(), nwnCharGen );
    if( ftd.exec() == QDialog::Accepted ) {
        nwnCharGen->getCharacter()->setFeatChoiceAtLvl( lvl, selectedIndex, ftd.getFeatChoice() );
        nwnCharGen->updateAll();
    }

    return true;
}
