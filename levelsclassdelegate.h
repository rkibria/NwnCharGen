#ifndef LEVELSCLASSDELEGATE_H
#define LEVELSCLASSDELEGATE_H

#include <QtWidgets>

class LevelsClassDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
};

#endif // LEVELSCLASSDELEGATE_H
