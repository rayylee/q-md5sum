#ifndef MD5THREAD_H
#define MD5THREAD_H

#include <QThread>
#include <QFile>

class MD5Thread : public QThread
{
    Q_OBJECT
signals:
    void dataAdded(const QString &filename, uint hashProgress);
    void md5Hashed(const QString &filename, const QByteArray &md5Data);
public:
    MD5Thread(const QString &filename);
    ~MD5Thread();
protected:
    void run();
private:
    QFile file;
};

#endif // MD5THREAD_H
