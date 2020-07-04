#ifndef LEVELSABLDELEGATE_H
#define LEVELSABLDELEGATE_H

#include <QtWidgets>

class LevelsAblDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                     const QStyleOptionViewItem &option, const QModelIndex &index) override;
};

#endif // LEVELSABLDELEGATE_H
