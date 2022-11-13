#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class MD5Thread;

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::Dialog *ui;
    QList<MD5Thread *> md5Threads;

private slots:
    void on_chooseFileButton_clicked();
    void on_hashButton_clicked();
    void onMD5ThreadDataAdded(const QString &filename, uint hashProgress);
    void onMD5ThreadMD5Hashed(const QString &filename, const QByteArray &md5Data);
    void onMD5ThreadFinished();
};

#endif // DIALOG_H
