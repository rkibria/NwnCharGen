#ifndef FEATSGAINEDDELEGATE_H
#define FEATSGAINEDDELEGATE_H

#include <QtWidgets>

class FeatsGainedDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
};

#endif // FEATSGAINEDDELEGATE_H
