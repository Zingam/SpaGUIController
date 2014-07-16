#include "scenedatamodel.h"

#include <QtCore/QFile>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QSpacerItem>

#include <QtCore/QDebug>


SceneDataModel::SceneDataModel(QString filePath, QString fileName, QMainWindow* mainWindow):
    _fileName(fileName),
    _mainWindow(mainWindow)
{
    QFile configFile(filePath + "/" + fileName);

    bool isOk;

    // Open config.xml file from device
    isOk = configFile.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!isOk) {
        this->showFatalError("Failed to open " + fileName + " in path: \n" + filePath);
    }

    // Set config.xml as content of the dom document
    QString errorMessage;
    int errorLine;
    int errorColumn;

    isOk = _dataDocument.setContent(&configFile, false, &errorMessage, &errorLine, &errorColumn);
    if (!isOk) {
        this->showFatalError("Failed to read " + fileName + " from device with:\n    "
                        + errorMessage
                        + "\nat line "
                        + QString::number(errorLine)
                        + ", column "
                        + QString::number(errorColumn));
    }
}

void SceneDataModel::load()
{

}

void SceneDataModel::save()
{

}

/*
 * Logging & error reporting methods
 */

///
/// \brief ConfigLoader::logToConsole
/// \param element
///
/// Logs a message to the console
///
void SceneDataModel::logToConsole(const QDomElement& element) const
{
    Q_UNUSED(element)
#ifdef NO_DEBUG_OUTPUT
    qDebug() << "Loaded element:" << element.tagName();
#endif
}

///
/// \brief ConfigLoader::logToConsole
/// \param element
///
/// Logs a message to the console
///
void SceneDataModel::logToConsole(const QDomAttr& attribute) const
{
    Q_UNUSED(attribute)
#ifdef NO_DEBUG_OUTPUT
    qDebug() << "Loaded attribute: " + attribute.name() + "=\"" + attribute.value() +"\"";
#endif
}

///
/// \brief ConfigLoader::showFatalError
/// \param errorMessage
///
/// Displays an error message and quits the application
///
void SceneDataModel::showFatalError(const QString errorMessage) const
{
    qDebug() << errorMessage;
    QMessageBox messageBox(_mainWindow);
    messageBox.setWindowTitle("ERROR");
    messageBox.setIcon(QMessageBox::Critical);
    messageBox.setText("Failed to load: " + _fileName);
    messageBox.setInformativeText(errorMessage);

    // MessageBox size hack
    QSpacerItem* horizontalSpacer = new QSpacerItem(300, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout = qobject_cast<QGridLayout*>(messageBox.layout());
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());

    messageBox.exec(); // Show modal
//    exit(-1);
}
