#include "configloader.h"

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QObject>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QSpacerItem>
#include <QtXml/QDomNodeList>
#include <QtXml/QDomAttr>


ConfigLoader::ConfigLoader(QString fileName)
{
    QFile configFile(fileName);

    bool isOk;

    // Open config.xml file from device
    isOk = configFile.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!isOk) {
        this->showFatalError(QObject::tr("Failed to open file in path: ") + "\n" + fileName);
    }

    // Set config.xml as content of the dom document
    QString errorMessage;
    int errorLine;
    int errorColumn;

    isOk = _configDocument.setContent(&configFile, false, &errorMessage, &errorLine, &errorColumn);
    if (!isOk) {
        this->showFatalError(QObject::tr("Failed to read")
                             + " config.xml "
                             + QObject::tr("from device with:")
                             + "\n    "                          // End of line
                             + errorMessage + "\n"               // End of line
                             + QObject::tr("at line ")
                             + QString::number(errorLine) + ", "
                             + QObject::tr("column") + " "
                             + QString::number(errorColumn));
    }

    // Start parsing the document
    qDebug() << "Begin parsing:" << fileName;

    QDomElement elementConfig = this->firstChildElement("config", _configDocument);

    // <settings> section
    QDomElement elementSettings = this->firstChildElement("settings", elementConfig);

    parseProgramSettings(elementSettings);

    // <indicators> section
    QDomNodeList listIndicatorsElement = elementConfig.elementsByTagName("indicators");

    bool foundNoValidLanguage = true;

    for (int i = 0; listIndicatorsElement.count() > i; i++) {
        QDomElement currentIndicatorsElement = listIndicatorsElement.at(i).toElement();
        if (currentIndicatorsElement.isNull()) {
            this->showFatalError(QObject::tr("Failed at loading") + " <indicators>");
        }
        this->logToConsole(currentIndicatorsElement);

        if (getAttributeValue("language", currentIndicatorsElement) == _programSettings.language) {
            parseIndicatorProperties(currentIndicatorsElement);
            foundNoValidLanguage = false;
            break;
        }
    }

    if (foundNoValidLanguage) {
        showFatalError(QObject::tr("Could not find valid language strings in") + " \"config.xml\"");
    }

    qDebug() << "Succeeded parsing:" << fileName;
}

/*
 * Helper methods
 */

///
/// \brief ConfigLoader::firstChildElement
/// \param tagName
/// \param document
/// \return
///
///  Helper function to parse the first Child element
///
QDomElement ConfigLoader::firstChildElement(const QString& tagName, QDomDocument& document) const
{
    QDomElement childElement = document.firstChildElement(tagName);
    if (childElement.isNull()) {
        this->showFatalError(QObject::tr("Failed to load") + " <"+ tagName + ">");
    }
    this->logToConsole(childElement);

    return childElement;
}

///
/// \brief ConfigLoader::firstChildElement
/// \param tagName
/// \param element
/// \return
///
///  Helper function to parse the first Child element
///
QDomElement ConfigLoader::firstChildElement(const QString& tagName, QDomElement& element) const
{
    QDomElement childElement = element.firstChildElement(tagName);
    if (childElement.isNull()) {
        this->showFatalError(QObject::tr("Failed to load") + " <"+ tagName + ">");
    }
    this->logToConsole(childElement);

    return childElement;
}

///
/// \brief ConfigLoader::nextSiblingElement
/// \param tagName
/// \param element
/// \return
///
/// Helper function to parse the next Sibling element
///
QDomElement ConfigLoader::nextSiblingElement(const QString& tagName, QDomElement& element) const
{
    QDomElement childElement = element.nextSiblingElement(tagName);
    if (childElement.isNull()) {
        this->showFatalError(QObject::tr("Failed to load") + " <"+ tagName + ">");
    }
    this->logToConsole(childElement);

    return childElement;
}

QString ConfigLoader::getAttributeValue(const QString& attributeName, QDomElement& element) const
{
    QDomAttr attribute = element.attributeNode(attributeName);
    if(attribute.isNull()) {
        this->showFatalError(QObject::tr("No valid attribute") + " <" + attributeName + "> "
                             + QObject::tr("found for") + " <" + element.tagName() + ">");
    }
    this->logToConsole(attribute);

    return attribute.value();
}

/*
 * Parsing methods
 */

void ConfigLoader::parseProgramSettings(QDomElement& element)
{
#ifdef DEBUG_OUTPUT
    qDebug() << "Parsing <" + element.tagName() + ">";
#endif // DEBUG_OUTPUT

    QDomElement currentElement;

    currentElement = this->firstChildElement("application", element);
    _programSettings.application.date = getAttributeValue("date", currentElement);
    _programSettings.application.name = getAttributeValue("name", currentElement);
    _programSettings.application.version = getAttributeValue("version", currentElement);

    currentElement = this->nextSiblingElement("datafile", currentElement);
    _programSettings.datafile.name = getAttributeValue("name", currentElement);
    _programSettings.datafile.path = getAttributeValue("path", currentElement);

    currentElement = this->nextSiblingElement("server", currentElement);
    _programSettings.server.ipV4Address = getAttributeValue("ipV4Address", currentElement);
    _programSettings.server.port = getAttributeValue("port", currentElement).toInt();

    currentElement = this->nextSiblingElement("language", currentElement);
    _programSettings.language = getAttributeValue("value", currentElement);

    currentElement = this->nextSiblingElement("assets", currentElement);
    _programSettings.assetsPath = getAttributeValue("path", currentElement);

    currentElement = this->nextSiblingElement("background", currentElement);
    _programSettings.backgroundImageFileName = getAttributeValue("imageSource", currentElement);

    currentElement = this->nextSiblingElement("highlight", currentElement);
    _programSettings.highlightOpacity = getAttributeValue("opacity", currentElement).toFloat();

    // <indicator>
    currentElement = this->nextSiblingElement("indicator", currentElement);

    QDomElement currentChildElement;

    currentChildElement = this->firstChildElement("opacity", currentElement);
    _programSettings.indicator.opacity = getAttributeValue("value", currentChildElement).toFloat();

    currentChildElement = this->nextSiblingElement("borderwidth", currentChildElement);
    _programSettings.indicator.borderwidth = getAttributeValue("value", currentChildElement).toInt();

    currentChildElement = this->nextSiblingElement("bordercolor", currentChildElement);
    _programSettings.indicator.bordercolor = getAttributeValue("value", currentChildElement);

    // <font>
    currentChildElement = this->nextSiblingElement("font", currentChildElement);
    _programSettings.indicator.fontFamily = getAttributeValue("family", currentChildElement);
    _programSettings.indicator.fontSize = getAttributeValue("size", currentChildElement).toInt();
    _programSettings.indicator.fontWeight = getAttributeValue("weight", currentChildElement);
    _programSettings.indicator.fontColor = getAttributeValue("color", currentChildElement);
    // </font>

    currentChildElement = this->nextSiblingElement("rectpadding", currentChildElement);
    _programSettings.indicator.rectPadding = getAttributeValue("value", currentChildElement).toFloat();

    // <alpha>
    currentChildElement = this->nextSiblingElement("alpha", currentChildElement);
    _programSettings.indicator.alphaSelected = getAttributeValue("selected", currentChildElement).toInt();
    _programSettings.indicator.alphaDisselected = getAttributeValue("disselected", currentChildElement).toInt();
    // </alpha>
    // </indicator>

    // Create a real font
    _programSettings.indicator.font = QFont();
    _programSettings.indicator.font.setFamily(_programSettings.indicator.fontFamily);
    _programSettings.indicator.font.setPointSize(_programSettings.indicator.fontSize);
    if ("bold" == _programSettings.indicator.fontWeight) {
        _programSettings.indicator.font.setBold(true);
    }
    else if ("italic" == _programSettings.indicator.fontWeight) {
        _programSettings.indicator.font.setItalic(true);
    }
    else if ("light" ==  _programSettings.indicator.fontWeight) {
        _programSettings.indicator.font.setWeight(QFont::Light);
    }
    else if ("demibold" ==_programSettings.indicator.fontWeight) {
         _programSettings.indicator.font.setWeight(QFont::DemiBold);
    }
    else if ("black" == _programSettings.indicator.fontWeight) {
         _programSettings.indicator.font.setWeight(QFont::Black);
    }
    else {
         _programSettings.indicator.font.setWeight(QFont::Normal);
    }
}

void ConfigLoader::parseIndicatorProperties(QDomElement& element)
{
#ifdef DEBUG_OUTPUT
    qDebug() << "Parsing <" + element.tagName() + ">";
#endif // DEBUG_OUTPUT

    QDomNodeList listChildElement = element.childNodes();

    for (int i = 0; listChildElement.count() > i; i++) {
        QDomElement currentChildElement = listChildElement.at(i).toElement();

        if (currentChildElement.isNull()) {
            this->showFatalError(QObject::tr("Failed to load") + " <item>");
        }
        this->logToConsole(currentChildElement);

        IndicatorProperties indicatorProperties;

        indicatorProperties.sensorId = getAttributeValue("sensorid", currentChildElement).toInt();
        indicatorProperties.text = getAttributeValue("text", currentChildElement);
        indicatorProperties.highlightImageFileName = getAttributeValue("image", currentChildElement);
        qreal x = getAttributeValue("x", currentChildElement).toFloat();
        qreal y = getAttributeValue("y", currentChildElement).toFloat();
        indicatorProperties.position = QPointF(x, y);
        indicatorProperties.temperatureTarget =
                getAttributeValue("temperaturetarget", currentChildElement).toDouble();

        _listIndicatorProperties.push_back(indicatorProperties);
    }
}

/*
 * Properties
 */

ProgramSettings ConfigLoader::getProgramSettings() const
{
    return _programSettings;
}

QList<IndicatorProperties> ConfigLoader::getIndicatorProperties() const
{
    return _listIndicatorProperties;
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
void ConfigLoader::logToConsole(const QDomElement& element) const
{
    Q_UNUSED(element)
#ifdef DEBUG_OUTPUT
    qDebug() << "Loaded element:" << element.tagName();
#endif
}

///
/// \brief ConfigLoader::logToConsole
/// \param element
///
/// Logs a message to the console
///
void ConfigLoader::logToConsole(const QDomAttr& attribute) const
{
    Q_UNUSED(attribute)
#ifdef DEBUG_OUTPUT
    qDebug() << "Loaded attribute: " + attribute.name() + "=\"" + attribute.value() +"\"";
#endif
}

///
/// \brief ConfigLoader::showFatalError
/// \param errorMessage
///
/// Displays an error message and quits the application
///
void ConfigLoader::showFatalError(const QString errorMessage) const
{
    qDebug() << errorMessage;
//    QMessageBox messageBox(_mainWindow);
    QMessageBox messageBox;
    messageBox.setWindowTitle(QObject::tr("ERROR"));
    messageBox.setIcon(QMessageBox::Critical);
    messageBox.setText(QString(QObject::tr(ERROR_MESSAGE_01)) + QString(ERROR_MESSAGE_02));
    messageBox.setInformativeText(errorMessage);

    // MessageBox size hack
    QSpacerItem* horizontalSpacer = new QSpacerItem(300, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout = qobject_cast<QGridLayout*>(messageBox.layout());
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());

    messageBox.exec(); // Show modal

    exit(-1);
}
