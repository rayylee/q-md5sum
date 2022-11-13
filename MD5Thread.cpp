#include <QCryptographicHash>

#include "MD5Thread.h"

MD5Thread::MD5Thread(const QString &filename) : file(filename)
{
    qDebug("MD5Thread(const QString &) called");
}

MD5Thread::~MD5Thread()
{
    qDebug("~MD5Thread() called");
}

void MD5Thread::run()
{
    if (file.open(QFile::ReadOnly)) {
        QCryptographicHash md5(QCryptographicHash::Md5);
        QByteArray buffer;
        qint64 addedDataSize = 0;
        qint64 totalBytes = file.size();
        while (!(buffer = file.read(4 * 1024)).isEmpty()) {
            md5.addData(buffer);
            emit dataAdded(file.fileName(), static_cast<uint>((addedDataSize += buffer.size()) * 100.0 / totalBytes));
            buffer.resize(0);
            if (addedDataSize == totalBytes) {
                break;
            }
        }
        emit md5Hashed(file.fileName(), md5.result().toHex());
        file.close();
    }
}
