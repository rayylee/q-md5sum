#include "dialog.h"
#include "ui_dialog.h"

#include <QDebug>
#include <QFileDialog>
#include <QCloseEvent>
#include <QMessageBox>
#include <QFileInfo>
#include <QListWidgetItem>
#include <QProgressBar>
#include <QGraphicsOpacityEffect>
#include <QMenuBar>

#include "MD5Thread.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    qDebug("Dialog(QWidget *) called");

    ui->setupUi(this);

    ui->chooseFileButton->setEnabled(true);
    ui->hashButton->setEnabled(false);
}

Dialog::~Dialog()
{
    delete ui;
    qDebug("~Dialog() called");
}

void Dialog::closeEvent(QCloseEvent *event)
{
    if (!this->md5Threads.isEmpty()) {
        foreach (MD5Thread *i, this->md5Threads) {
            if (!i->isFinished()) {
                QMessageBox::critical(this, "Tip", "Calculating the MD5 value of the file！");
                event->ignore();
            }
        }
    }
}

void Dialog::on_chooseFileButton_clicked()
{
    ui->md5Browser->clear();     // 清空显示区
    ui->md5ListWidget->clear();  // 清空文件列表

    // QStringList paths = QFileDialog::getOpenFileNames(this, "Open File");

    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
    // fileDialog->setOption(QFileDialog::DontUseNativeDialog, true);
    fileDialog->setOption(QFileDialog::ReadOnly, true);
    // fileDialog->setMinimumSize(1024, 600);

    if (fileDialog->exec() == QDialog::Accepted) {
        QStringList paths = fileDialog->selectedFiles();
        if (paths.count() != 0) {
            for (int i = 0; i < paths.count(); i++) {
                QListWidgetItem *item = new QListWidgetItem;  // 新建一个项
                item->setWhatsThis(paths[i]);

                if (paths[i].length() > 80) {
                    item->setText( "..." + paths[i].right(80));
                } else {
                    item->setText(paths[i]);
                }

                QProgressBar *pBar = new QProgressBar;
                pBar->setRange(0, 100);
                pBar->setValue(0);
                QGraphicsOpacityEffect *opacity = new QGraphicsOpacityEffect;
                opacity->setOpacity(0.4);           // 透明范围同窗口透明度
                pBar->setGraphicsEffect(opacity);

                this->ui->md5ListWidget->addItem(item);
                this->ui->md5ListWidget->setItemWidget(item, pBar);
            }
        }
    }

    ui->hashButton->setEnabled(true);
}

void Dialog::on_hashButton_clicked()
{   
    int nCnt = this->ui->md5ListWidget->count();

    for (int i = 0; i < nCnt; ++i) {
        QListWidgetItem* item = this->ui->md5ListWidget->item(i);
        if (item) {
            QString f = item->whatsThis();
            MD5Thread *t = new MD5Thread(f);
            this->md5Threads.append(t);
            connect(t, &MD5Thread::dataAdded, this, &Dialog::onMD5ThreadDataAdded);
            connect(t, &MD5Thread::md5Hashed, this, &Dialog::onMD5ThreadMD5Hashed);
            connect(t, &MD5Thread::finished, this, &Dialog::onMD5ThreadFinished);
            t->start();
        }
    }

    ui->hashButton->setEnabled(false);
    ui->chooseFileButton->setEnabled(false);
}

void Dialog::onMD5ThreadDataAdded(const QString &filename, uint hashProgress)
{
    int nCnt = this->ui->md5ListWidget->count();
    for (int i = 0; i < nCnt; ++i) {
        QListWidgetItem* item = this->ui->md5ListWidget->item(i);
        if (item && !filename.compare(item->whatsThis())) {
            auto pBar = dynamic_cast<QProgressBar *>(this->ui->md5ListWidget->itemWidget(item));
            pBar->setValue(hashProgress);
        }
    }
}

void Dialog::onMD5ThreadMD5Hashed(const QString &filename, const QByteArray &md5Data)
{
    int nCnt = this->ui->md5ListWidget->count();
    for (int i = 0; i < nCnt; ++i) {
        QListWidgetItem* item = this->ui->md5ListWidget->item(i);
        if (item && !filename.compare(item->whatsThis())) {
            QFileInfo fileInfo = QFileInfo(item->whatsThis());
            ui->md5Browser->appendPlainText(md5Data + " "+ fileInfo.fileName());
        }
    }    
}

void Dialog::onMD5ThreadFinished()
{
    foreach (MD5Thread *i, this->md5Threads) {
        if (i->isFinished()) {
            i->deleteLater();
            this->md5Threads.removeOne(i);
        }
    }

    if (this->md5Threads.isEmpty()) {
        ui->chooseFileButton->setEnabled(true);
    }
}
