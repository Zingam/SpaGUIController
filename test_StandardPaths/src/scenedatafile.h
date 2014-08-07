#ifndef SCENEDATAFILE_H
#define SCENEDATAFILE_H

#include <QtCore/QObject>
#include <QtCore/QString>

class SceneDataFile : public QObject
{
    Q_OBJECT
public:
    explicit SceneDataFile(QString sceneDataFilePath,
                           QString sceneDataFileName,
                           QObject* parent = 0);

private:
    QString _sceneDataFileName;

signals:

public slots:
    void exportTo(QString filePath);
    void importFrom(QString filePath);
};

#endif // SCENEDATAFILE_H
