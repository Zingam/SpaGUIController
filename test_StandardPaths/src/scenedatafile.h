#ifndef SCENEDATAFILE_H
#define SCENEDATAFILE_H

#include <QtCore/QObject>
#include <QtCore/QString>

#include <exception>


class ExceptionInitialization : public std::exception
{
public:
    ExceptionInitialization(QString message) : _message(message) {}

    QString getMessage() { return _message; }

private:
    QString _message;
};

class SceneDataFile : public QObject
{
    Q_OBJECT
private:
    explicit SceneDataFile(QString sceneDataFilePath,
                           QString sceneDataFileName,
                           QObject* parent = 0);

    // Since this class is a singleton, delete all:
    SceneDataFile() = delete; // Default constructor
    SceneDataFile(const SceneDataFile&) = delete; // Copy constructor
    SceneDataFile& operator=(const SceneDataFile&) = delete; // Copy assignment operator
    SceneDataFile(SceneDataFile&&) = delete; // Move constructor
    SceneDataFile& operator=(SceneDataFile&&) = delete; // Move assignment operator

public:
    virtual ~SceneDataFile();

public:
    static SceneDataFile* getInstance(QString sceneDataFilePath,
                                      QString sceneDataFileName,
                                      QObject* parent);
private:
    QString _sceneDataFileName;

signals:

public slots:
    void exportTo(QString filePath);
    void importFrom(QString filePath);
};

#endif // SCENEDATAFILE_H
