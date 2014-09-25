#ifndef SINGLEINSTANCEAPPLICATION_H
#define SINGLEINSTANCEAPPLICATION_H

#include <QtCore/QSharedMemory>
#include <QtNetwork/QLocalServer>
#include <QtWidgets/QApplication>


class SingleInstanceApplication : public QApplication
{
    Q_OBJECT
public:
    enum class SingletonType {
        LocalSocket,
        SharedMemory
    };

public:
    explicit SingleInstanceApplication(int& argc,
                                       char* argv[],
                                       const QString& uniqueKey,
                                       SingletonType singletonType=SingletonType::LocalSocket);
public:
    bool isInstanciated();

private:
    bool useLocalSocket();
    bool useSharedMemory();

private:
    bool _isInstanciated = false;
    QLocalServer* _localServer;
    QSharedMemory _sharedMemory;
    const QString& _uniqueKey;

private slots:
    void onNewConnection();
};

#endif // SINGLEINSTANCEAPPLICATION_H
