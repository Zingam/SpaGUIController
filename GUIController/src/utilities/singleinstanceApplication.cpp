#include "singleapplicationinstance.h"

#include <QtCore/QDebug>
#include <QtCore/QSharedMemory>
#include <QtNetwork/QLocalSocket>
#include <QtWidgets/QMessageBox>

SingleInstanceApplication::SingleInstanceApplication(int &argc,
                                                     char *argv[],
                                                     const QString& uniqueKey,
                                                     SingletonType singletonType) :
    QApplication(argc, argv),
    _uniqueKey(uniqueKey)
{
    switch (singletonType) {
    case SingletonType::LocalSocket:
        _isInstanciated = this->useLocalSocket();
        break;
    case SingletonType::SharedMemory:
        _isInstanciated = this->useSharedMemory();
        break;
    }
}

bool SingleInstanceApplication::isInstanciated()
{
    return _isInstanciated;
}

bool SingleInstanceApplication::useLocalSocket()
{
    QLocalSocket localSocket;
    localSocket.connectToServer(_uniqueKey);
    
    // If local socket connects successfully there is another running instance
    bool isConnected = localSocket.waitForConnected(500);
    if (isConnected) {
        return isConnected;
    }

    // Start a local server and listen for incomming connections
    _localServer = new QLocalServer(this);
    bool isOk = connect(_localServer,
                        &QLocalServer::newConnection,
                        this,
                        &SingleInstanceApplication::onNewConnection);
    Q_ASSERT(isOk);
    Q_UNUSED(isOk);

    _localServer->listen(_uniqueKey);

    return isConnected;
}

bool SingleInstanceApplication::useSharedMemory()
{
    _sharedMemory.setKey(_uniqueKey);

    // If shared memory is created and attached successfuly,
    // no other application instace is running
    bool hasNoPreviousInstance = _sharedMemory.create(1);

    return !hasNoPreviousInstance;
}

void SingleInstanceApplication::onNewConnection()
{
    // Nothing to do here...
}
