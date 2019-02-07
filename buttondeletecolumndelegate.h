#ifndef BUTTONDELETECOLUMNDELEGATE_H
#define BUTTONDELETECOLUMNDELEGATE_H

#include <QStyledItemDelegate>
#include <QApplication>
#include <QDialog>
#include <QMouseEvent>
#include <QDebug>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QDesktopWidget>
#include <QFile>

class ButtonDeleteColumnDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit ButtonDeleteColumnDelegate(QObject *parent = nullptr);
    ~ButtonDeleteColumnDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

    QString DisplayMethod;
    QString Id = "";
    QList<int> integerListDeleteRow;

    QAbstractItemModel *modelDeleteRow;
    int DeleteRow = -1;
    QDialog * DialogDeleteROW = new QDialog();
    QGridLayout *layout = new QGridLayout;
    QLabel *IdLabel = new QLabel(tr("Id записи в базе:"));
    QLineEdit *IdLineEdit = new QLineEdit;

    QLineEdit *PointNameLineEdit = new QLineEdit;
    QPushButton *DeleteButton = new QPushButton(tr("Удалить"));
    QLabel *PointNameLabel = new QLabel(tr("Значение коэффициента:"));

private slots:
    void DeleteFunction();
};

#endif // BUTTONDELETECOLUMNDELEGATE_H
