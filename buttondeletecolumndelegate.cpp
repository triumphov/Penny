#include "buttondeletecolumndelegate.h"
#include <QPainter>

ButtonDeleteColumnDelegate::ButtonDeleteColumnDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
    integerListDeleteRow.clear();
    modelDeleteRow = nullptr;
}

ButtonDeleteColumnDelegate::~ButtonDeleteColumnDelegate()
{

}

void ButtonDeleteColumnDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionButton button;
    QRect r = option.rect;//getting the rect of the cell
    int x,y,w,h;
    x = r.left() + r.width()/2 - 32/2;//the X coordinate
    y = r.top();//the Y coordinate
    w = 32;//button width
    h = 30;//button height
    button.rect = QRect(x,y,w,h);
    button.icon = QApplication::style()->standardIcon(QStyle::SP_TrashIcon);
    button.iconSize = QSize(16,16);
    button.state = QStyle::State_Enabled;
    QApplication::style()->drawControl( QStyle::CE_PushButton, &button, painter);
}

bool ButtonDeleteColumnDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if( event->type() == QEvent::MouseButtonRelease )
    {
        integerListDeleteRow.clear();
        modelDeleteRow = nullptr;

        DialogDeleteROW->setWindowTitle(tr("Удалить коэффициент"));
        DialogDeleteROW->setWindowFlags(DialogDeleteROW->windowFlags() & ~Qt::WindowContextHelpButtonHint);
        Id = model->data(model->index(index.row(),2),Qt::ToolTipRole).toString();
        modelDeleteRow = model;
        integerListDeleteRow.append(index.row());

        IdLineEdit->setText(Id);
        PointNameLineEdit->setText(model->data(model->index(index.row(),0),Qt::DisplayRole).toString());
        IdLineEdit->setEnabled(false);
        PointNameLineEdit->setEnabled(false);
        IdLabel->hide();
        IdLineEdit->hide();
        layout->addWidget(IdLabel,0,0);
        layout->addWidget(PointNameLabel,1,0);
        layout->addWidget(IdLineEdit,0,1);
        layout->addWidget(PointNameLineEdit,1,1);

        connect(DeleteButton,SIGNAL(clicked(bool)),SLOT(DeleteFunction()));
        layout->addWidget(DeleteButton,2,0,1,2);
        DialogDeleteROW->setLayout(layout);
        QRect screenGeometry = QApplication::desktop()->screenGeometry();
        int x = (screenGeometry.width() - DialogDeleteROW->width()) / 2;
        int y = (screenGeometry.height() - DialogDeleteROW->height()) / 2;
        DialogDeleteROW->move(x,y);
        DialogDeleteROW->show();

    }
    return true;
}


void ButtonDeleteColumnDelegate::DeleteFunction() {
    for (int i=0;i<integerListDeleteRow.size();i++) {
        modelDeleteRow->removeRow(integerListDeleteRow.takeAt(i));
    }
    integerListDeleteRow.clear();
    DialogDeleteROW->close();
}
