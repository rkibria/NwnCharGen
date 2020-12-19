#ifndef FEATCHOICEDELEGATE_H
#define FEATCHOICEDELEGATE_H

#include <QtWidgets>

class FeatChoiceDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // FEATCHOICEDELEGATE_H
