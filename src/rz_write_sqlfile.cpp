#include "includes/rz_write_sqlfile.hpp"
#include "includes/rz_config.h"

const QString PLUGIN_SHORTNAME = PROG_EXEC_NAME;
const QString PLUGIN_NAME = PROG_NAME;
const QString PLUGIN_VERSION = PROG_VERSION;
const QString PLUGIN_DESCRIPTION = PROG_DESCRIPTION;

Rz_writeSQLfile::Rz_writeSQLfile(QObject *parent)
{
    Q_UNUSED(parent);
}

Rz_writeSQLfile::~Rz_writeSQLfile() {}

QString Rz_writeSQLfile::getPluginNameShort()
{
    return PLUGIN_SHORTNAME;
}

QString Rz_writeSQLfile::getPluginNameLong()
{
    return PLUGIN_NAME;
}

QString Rz_writeSQLfile::getPluginVersion()
{
    return PLUGIN_NAME + "-v" + PLUGIN_VERSION;
}

QString Rz_writeSQLfile::getPluginDescription()
{
    return PLUGIN_DESCRIPTION;
}

std::tuple<bool, std::string> Rz_writeSQLfile::parseFile(QMap<QString, QString> &configMap,
                                                         QString empty)
{
    QString pathToDb = configMap.value("pathToDb");

    std::tie(oknok, msg) = openDefaultMetaDb(pathToDb);

    if (oknok == false) {
        return std::make_tuple(oknok, "Rz_writeSQLfile::parseFile::" + msg);
    }

    std::tie(oknok, msg) = setDefaultMetaKeys();
    if (oknok == false) {
        return std::make_tuple(oknok, "Rz_writeSQLfile::parseFile::" + msg);
    }
    return std::make_tuple(oknok, "Rz_writeSQLfile::parseFile");
}

std::tuple<bool, std::string> Rz_writeSQLfile::writeFile(QMap<QString, QString> mapParseKeys,
                                                         QMap<QString, QString> mapFileAttribs,
                                                         QString pathToFile)
{
    qDebug() << "Rz_writeSQLfile::writeFile: " << pathToFile;
    std::tie(oknok, msg) = createSQLfiles();
    /*
    QRegularExpression re(R"(^(\w+)\.(\w+)\.(\w+))");
    QRegularExpressionMatch match = re.match("Exif.GPSInfo.GPSLongitudeRef");

    if (match.hasMatch()) {
        QString one = match.captured(1);   // day == "08"
        QString two = match.captured(2);   // month == "12"
        QString three = match.captured(3); // year == "1985"
        qDebug() << "1-2-3 " << one << "-" << two << "-" << three;
    }
    qDebug() << "Rz_writeSQLfile::writeFile: the End";

    */
    return std::make_tuple(oknok, "Rz_writeSQLfile::writeFile: " + msg);
}

void Rz_writeSQLfile::setHashMap(const QHash<QString, QString> defaultMetaKeys, QString type)
{
    if (type.contains("IPTC")) {
        iptcMetaTags = defaultMetaKeys;
        return;
    }
    if (type.contains("XMP")) {
        xmpMetaTags = defaultMetaKeys;
        return;
    }
    if (type.contains("EXIF")) {
        exifMetaTags = defaultMetaKeys;
    }
}

QHash<QString, QString> Rz_writeSQLfile::getHashMap(QString type) {}

void Rz_writeSQLfile::doClose()
{
    metaDb.closeDb();
}

std::tuple<bool, std::string> Rz_writeSQLfile::openDefaultMetaDb(QString &pathToSQLiteDb)
{
    return metaDb.openSQLiteDatabase(pathToSQLiteDb);
}

std::tuple<bool, std::string> Rz_writeSQLfile::setDefaultMetaKeys()
{
    addMetaKeys(metaDb.getTableFields("EXIF"), "EXIF");
    addMetaKeys(metaDb.getTableFields("IPTC"), "IPTC");
    addMetaKeys(metaDb.getTableFields("XMP"), "XMP");
    /*
    QList fieldNames = metaDb.getTableFields("EXIF");

    QList<QString>::iterator i;
    for (i = fieldNames.begin(); i != fieldNames.end(); ++i) {
        qDebug() << "fieldName: " << *i;
        exifMetaTags.insert(*i, "");
    }

    fieldNames = metaDb.getTableFields("IPTC");

    for (i = fieldNames.begin(); i != fieldNames.end(); ++i) {
        qDebug() << "fieldName: " << *i;
        iptcMetaTags.insert(*i, "");
    }

    fieldNames = metaDb.getTableFields("XMP");

    for (i = fieldNames.begin(); i != fieldNames.end(); ++i) {
        qDebug() << "fieldName: " << *i;
        xmpMetaTags.insert(*i, "");
    }
*/
    return std::make_tuple(true, "");
}

void Rz_writeSQLfile::addMetaKeys(const QList<QString> &keys, const QString &type)
{
    QList<QString>::iterator i;

    for (const QString &key : keys) {
        if (type.contains("IPTC")) {
            iptcMetaTags.insert(key, "");
            return;
        } else if (type.contains("XMP")) {
            xmpMetaTags.insert(key, "");
            return;
        } else if (type.contains("EXIF")) {
            exifMetaTags.insert(key, "");
        }
    }
}

std::tuple<bool, std::string> Rz_writeSQLfile::createSQLfiles(const QString pathToSqlFilesFolder)
{
    qDebug() << "Rz_writeSQLfile::createSQLfiles: " << pathToSqlFilesFolder;
    std::tie(oknok, msg) = isTargetExist(QFile(pathToSqlFilesFolder), "dir");
    if (!oknok) {
        return std::make_tuple(false, msg);
    }

    // Exif.GPSInfo.GPSLongitudeRef
    QRegularExpression re(R"(^(\w+)\.(\w+)\.(\w+))");
    QString tableNames = "CREATE TABLE IF NOT EXISTS ";

    // EXIF
    std::tie(oknok, msg) = isTargetExist(QFile(pathToSqlFilesFolder + "/EXIF"), "dir");
    if (!oknok) {
        return std::make_tuple(false, msg);
    }

    tableNames.append("photo_exif (Id INTEGER PRIMARY KEY");
    for (auto i = exifMetaTags.begin(); i != exifMetaTags.end(); ++i) {
        qDebug() << i.key() << ": " << i.value();

        QRegularExpressionMatch match = re.match(i.key());

        if (match.hasMatch()) {
            QString one = match.captured(1);   // Exif
            QString two = match.captured(2);   // GPSInfo
            QString three = match.captured(3); // GPSLongitudeRef
            qDebug() << "1-2-3 " << one << "-" << two << "-" << three;
            tableNames.append(", " + three.toLower() + " TEXT");
        }
    }
    tableNames.append(");");
    qDebug() << "SQL: " << tableNames;

    return std::make_tuple(false, "Rz_convertImage::isTargetExist:: no");
}

std::tuple<bool, std::string> Rz_writeSQLfile::isTargetExist(const QFile &pathToTarget,
                                                             const QString type)
{
    qDebug() << "Rz_writeSQLfile::isTargetExist: " << pathToTarget.fileName();
    QFileInfo fInfo(pathToTarget);
    qDebug() << "Rz_writeSQLfile::fInfo: " << fInfo.absoluteFilePath();

    if (type.contains("dir")) {
        if (!pathToTarget.exists()) {
            qDebug() << "createDirectories(fInfo.absolutePath().toStdString(): "
                     << fInfo.absolutePath().toStdString();
            std::tie(oknok, msg) = createDirectories(fInfo.absoluteFilePath().toStdString());
        }
        if (fInfo.isDir() && fInfo.isWritable()) {
            return std::make_tuple(true, "Rz_convertImage::isTargetExist::dir: yes");
        } else {
            return std::make_tuple(true, "Rz_convertImage::isTargetExist::dir: no: " + msg);
        }
    }
    if (!pathToTarget.exists()) {
        return std::make_tuple(false, "Rz_convertImage::isTargetExist:: no");
    }
    if (type.contains("file") && fInfo.isFile() && fInfo.isWritable()) {
        return std::make_tuple(true, "Rz_convertImage::isTargetExist::file: yes");
    }
    return std::make_tuple(false, "Rz_convertImage::isTargetExist:: no");
}

std::tuple<bool, std::string> Rz_writeSQLfile::createDirectories(
    const std::filesystem::path &p) noexcept
{
    qDebug() << "Rz_writeSQLfile::createDirectories:: " << p.c_str();
    std::filesystem::path nested = p;

    try {
        if (std::filesystem::create_directories(nested)) {
            return std::make_tuple(
                true, "Rz_writeSQLfile::createDirectories: Nested directories created successfully");

        } else {
            return std::make_tuple(
                false, "Rz_writeSQLfile::createDirectories: Failed to create nested directories");
            // std::cout << ec.message() << '\n';
        }
    } catch (const std::exception &ex) {
        std::string msg = "Rz_writeSQLfile::createDirectories: ";
        msg.append(ex.what());
        return std::make_tuple(false, msg);
    }

    return std::make_tuple(false, "Rz_writeSQLfile::createDirectories: failed");
}
