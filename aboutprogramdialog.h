#ifndef ABOUTPROGRAMDIALOG_H
#define ABOUTPROGRAMDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QImage>
#include <QImageReader>
#include <QIcon>

class AboutProgramDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AboutProgramDialog(QDialog *parent = nullptr);
    ~AboutProgramDialog();
    QGridLayout *layout = new QGridLayout(this);

};

#endif // ABOUTPROGRAMDIALOG_H
