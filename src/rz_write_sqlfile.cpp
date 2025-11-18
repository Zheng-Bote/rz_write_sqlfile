#include "includes/rz_write_sqlfile.hpp"


Rz_writeSQLfile::Rz_writeSQLfile(QObject *parent)
{
    Q_UNUSED(parent);
}

Rz_writeSQLfile::~Rz_writeSQLfile() = default;

QString Rz_writeSQLfile::getPluginNameShort()
{
    const std::string ret = format("{}", project_name);
    return ret.c_str();
}

QString Rz_writeSQLfile::getPluginNameLong()
{
    const std::string ret = format("{}", prog_longname);
    return ret.c_str();
}

QString Rz_writeSQLfile::getPluginVersion()
{
    std::string ret = format("{}", project_name);
    ret.append("-v");
    ret.append(project_version);
    return ret.c_str();
}

QString Rz_writeSQLfile::getPluginDescription()
{
    const std::string ret = format({}, project_description);
    return ret.c_str();
}

std::tuple<bool, std::string> Rz_writeSQLfile::parseFile(QMap<QString, QString> &configMap,
                                                         QString empty)
{
    return std::make_tuple(true, "Rz_writeSQLfile::parseFile");
}

std::tuple<bool, std::string> Rz_writeSQLfile::writeFile(QMap<QString, QString> mapParseKeys,
                                                         QMap<QString, QString> mapFileAttribs,
                                                         QString pathToFile)
{
    //qDebug() << "Rz_writeSQLfile::writeFile: " << pathToFile;
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
    QString debugMsg;
    //qDebug() << "Rz_writeSQLfile::createSQLfiles: " << pathToSqlFilesFolder;
    std::tie(oknok, msg) = isTargetExist(QFile(pathToSqlFilesFolder), "dir");
    if (!oknok) {
        debugMsg =
            QString(" %1 %2 %3").arg(__FILE__, __FUNCTION__).arg(__LINE__);
        msg + debugMsg.toStdString();
        return std::make_tuple(false, msg);
    }

    createPhotoTable(pathToSqlFilesFolder);
    createExifTable(pathToSqlFilesFolder);
    createIptcTable(pathToSqlFilesFolder);
    createXmpTable(pathToSqlFilesFolder);

    debugMsg = QString(" %1 %2 %3").arg(__FILE__, __FUNCTION__).arg(__LINE__);
    msg = "";
    msg + debugMsg.toStdString();
    return std::make_tuple(true, msg);
}

std::tuple<bool, std::string> Rz_writeSQLfile::createPhotoTable(
    const QString &pathToSqlFilesFolder)
{
    QString tableNames = "CREATE TABLE IF NOT EXISTS ";

    // EXIF

    QString photoFile = pathToSqlFilesFolder + "/PHOTO/some.sql";

    std::tie(oknok, msg) =
        isTargetExist(QFile(pathToSqlFilesFolder + "/PHOTO"), "dir");
    if (!oknok)
    {
        debugMsg =
            QString(" %1 %2 %3").arg(__FILE__, __FUNCTION__).arg(__LINE__);
        msg + debugMsg.toStdString();
        return std::make_tuple(false, msg);
    }

    tableNames.append("photo_image (\n\tid INTEGER PRIMARY KEY AUTOINCREMENT,");
    tableNames.append("\n\tfilename TEXT,");
    tableNames.append("\n\tfilepath TEXT");

    tableNames.append("\n);\n");

    writeFile(photoFile, tableNames);
    return std::make_tuple(true, "Rz_writeSQLfile::createPhotoTable");
}

std::tuple<bool, std::string> Rz_writeSQLfile::createIptcTable(
    const QString &pathToSqlFilesFolder)
{
    // Exif.GPSInfo.GPSLongitudeRef
    const QRegularExpression re(R"(^(\w+)\.(\w+)\.(\w+))");
    QString tableNames = "CREATE TABLE IF NOT EXISTS ";

    // EXIF

    QString iptcFile = pathToSqlFilesFolder + "/IPTC/some.sql";


    std::tie(oknok, msg) =
        isTargetExist(QFile(pathToSqlFilesFolder + "/IPTC"), "dir");
    if (!oknok)
    {
        debugMsg =
            QString(" %1 %2 %3").arg(__FILE__, __FUNCTION__).arg(__LINE__);
        msg + debugMsg.toStdString();
        return std::make_tuple(false, msg);
    }

    tableNames.append("photo_iptc (\n\tid INTEGER PRIMARY KEY AUTOINCREMENT,");
    tableNames.append("\n\tphoto_id INTEGER");
    for (auto i = iptcMetaTags.begin(); i != iptcMetaTags.end(); ++i)
    {
        //qDebug() << i.key() << ": " << i.value();

        const QRegularExpressionMatch match = re.match(i.key());

        if (match.hasMatch())
        {
            const QString one = match.captured(1);   // Exif
            const QString two = match.captured(2);   // GPSInfo
            const QString three = match.captured(3); // GPSLongitudeRef
            tableNames.append(",\n\t" + three.toLower() + " TEXT");
        }
    }
    tableNames.append(",\n\tFOREIGN KEY(photo_id) REFERENCES photo_image(id)");
    tableNames.append("\n);\n");

    writeFile(iptcFile, tableNames);
    writeIptcData(iptcFile);
    return std::make_tuple(true, "Rz_writeSQLfile::createIptcTable");
}

std::tuple<bool, std::string> Rz_writeSQLfile::createXmpTable(
    const QString &pathToSqlFilesFolder)
{
    // Exif.GPSInfo.GPSLongitudeRef
    const QRegularExpression re(R"(^(\w+)\.(\w+)\.(\w+))");
    QString tableNames = "CREATE TABLE IF NOT EXISTS ";

    // EXIF
    QString xmpFile = pathToSqlFilesFolder + "/XMP/some.sql";
    std::tie(oknok, msg) =
        isTargetExist(QFile(pathToSqlFilesFolder + "/XMP"), "dir");
    if (!oknok)
    {
        debugMsg =
            QString(" %1 %2 %3").arg(__FILE__, __FUNCTION__).arg(__LINE__);
        msg + debugMsg.toStdString();
        return std::make_tuple(false, msg);
    }

    tableNames.append("photo_xmp (\n\tid INTEGER PRIMARY KEY AUTOINCREMENT,");
    tableNames.append("\n\tphoto_id INTEGER");
    for (auto i = xmpMetaTags.begin(); i != xmpMetaTags.end(); ++i)
    {
        //qDebug() << i.key() << ": " << i.value();

        const QRegularExpressionMatch match = re.match(i.key());

        if (match.hasMatch())
        {
            const QString one = match.captured(1);   // Exif
            const QString two = match.captured(2);   // GPSInfo
            const QString three = match.captured(3); // GPSLongitudeRef
            tableNames.append(",\n\t" + three.toLower() + " TEXT");
        }
    }
    tableNames.append(",\n\tFOREIGN KEY(photo_id) REFERENCES photo_image(id)");
    tableNames.append("\n);\n");

    writeFile(xmpFile, tableNames);
    writeXmpData(xmpFile);
    return std::make_tuple(true, "Rz_writeSQLfile::createXmpTable");
}

std::tuple<bool, std::string> Rz_writeSQLfile::createExifTable(
    const QString &pathToSqlFilesFolder)
{
    // Exif.GPSInfo.GPSLongitudeRef
    const QRegularExpression re(R"(^(\w+)\.(\w+)\.(\w+))");
    QString tableNames = "CREATE TABLE IF NOT EXISTS ";

    // EXIF
    QString exifFile = pathToSqlFilesFolder + "/EXIF/some.sql";
    std::tie(oknok, msg) =
        isTargetExist(QFile(pathToSqlFilesFolder + "/EXIF"), "dir");
    if (!oknok)
    {
        debugMsg =
            QString(" %1 %2 %3").arg(__FILE__, __FUNCTION__).arg(__LINE__);
        msg + debugMsg.toStdString();
        return std::make_tuple(false, msg);
    }

    tableNames.append("photo_exif (\n\tid INTEGER PRIMARY KEY AUTOINCREMENT,");
    tableNames.append("\n\tphoto_id INTEGER");
    for (auto i = exifMetaTags.begin(); i != exifMetaTags.end(); ++i)
    {
        //qDebug() << i.key() << ": " << i.value();

        const QRegularExpressionMatch match = re.match(i.key());

        if (match.hasMatch())
        {
            const QString one = match.captured(1);   // Exif
            const QString two = match.captured(2);   // GPSInfo
            const QString three = match.captured(3); // GPSLongitudeRef
            tableNames.append(",\n\t" + three.toLower() + " TEXT");
        }
    }
    tableNames.append(",\n\tFOREIGN KEY(photo_id) REFERENCES photo_image(id)");
    tableNames.append("\n);\n");

    writeFile(exifFile, tableNames);
    writeExifData(exifFile);
    return std::make_tuple(true, "Rz_writeSQLfile::createExifTable");
}

void Rz_writeSQLfile::writeFile(QString &file, QString &content)
{
    QFile fileOut(file);
    if (!fileOut.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Rz_writeSQLfile::writeFile:: not open: " << file;
        return;
    }
    QTextStream out(&fileOut);
    out << content << "\n";
    fileOut.flush();
    fileOut.close();
}


std::tuple<bool, std::string> Rz_writeSQLfile::writeExifData(
    const QString &pathToSqlFilesFolder)
{
    QFile fileOut(pathToSqlFilesFolder);
    if (!fileOut.open(QIODeviceBase::Append | QIODevice::Text))
    {
        qDebug() << "Rz_writeSQLfile::writeExifData:: not open: "
                 << pathToSqlFilesFolder;
        return std::make_tuple(false, "Rz_writeSQLfile::writeExifData");
    }
    QTextStream out(&fileOut);
    const QRegularExpression re(R"(^(\w+)\.(\w+)\.(\w+))");
    QString keys, vals;
    QString queryPrepare = "INSERT INTO photo_exif (";

    for (auto i = exifMetaTags.begin(); i != exifMetaTags.end(); ++i)
    {
        const QRegularExpressionMatch match = re.match(i.key());

        if (match.hasMatch())
        {
            //const QString one = match.captured(1);   // Exif
            //const QString two = match.captured(2);   // GPSInfo
            const QString three = match.captured(3); // GPSLongitudeRef
            keys.append(three.toLower() + ",");
            vals.append("'" + i.value() + "',");
        }
    }

    keys.chop(1);
    vals.chop(1);
    queryPrepare.append(keys + ") VALUES(" + vals + ");");

    out << queryPrepare;
    fileOut.flush();
    fileOut.close();
    return std::make_tuple(true, "Rz_writeSQLfile::writeExifData");
}

std::tuple<bool, std::string> Rz_writeSQLfile::writeIptcData(
    const QString &pathToSqlFilesFolder)
{
    QFile fileOut(pathToSqlFilesFolder);
    if (!fileOut.open(QIODeviceBase::Append | QIODevice::Text))
    {
        qDebug() << "Rz_writeSQLfile::writeIptcData:: not open: "
                 << pathToSqlFilesFolder;
        return std::make_tuple(false, "Rz_writeSQLfile::writeIptcData");
    }
    QTextStream out(&fileOut);
    const QRegularExpression re(R"(^(\w+)\.(\w+)\.(\w+))");
    QString keys, vals;
    QString queryPrepare = "INSERT INTO photo_iptc (";

    for (auto i = iptcMetaTags.begin(); i != iptcMetaTags.end(); ++i)
    {
        const QRegularExpressionMatch match = re.match(i.key());

        if (match.hasMatch())
        {
            //const QString one = match.captured(1);   // Exif
            //const QString two = match.captured(2);   // GPSInfo
            const QString three = match.captured(3); // GPSLongitudeRef
            keys.append(three.toLower() + ",");
            vals.append("'" + i.value() + "',");
        }
    }

    keys.chop(1);
    vals.chop(1);
    queryPrepare.append(keys + ") VALUES(" + vals + ");");

    out << queryPrepare;
    fileOut.flush();
    fileOut.close();
    return std::make_tuple(true, "Rz_writeSQLfile::writeIptcData");
}

std::tuple<bool, std::string> Rz_writeSQLfile::writeXmpData(
    const QString &pathToSqlFilesFolder)
{
    QFile fileOut(pathToSqlFilesFolder);
    if (!fileOut.open(QIODeviceBase::Append | QIODevice::Text))
    {
        qDebug() << "Rz_writeSQLfile::writeXmpData:: not open: "
                 << pathToSqlFilesFolder;
        return std::make_tuple(false, "Rz_writeSQLfile::writeXmpData");
    }
    QTextStream out(&fileOut);
    const QRegularExpression re(R"(^(\w+)\.(\w+)\.(\w+))");
    QString keys, vals;
    QString queryPrepare = "INSERT INTO photo_xmp (";

    for (auto i = xmpMetaTags.begin(); i != xmpMetaTags.end(); ++i)
    {
        const QRegularExpressionMatch match = re.match(i.key());

        if (match.hasMatch())
        {
            //const QString one = match.captured(1);   // Exif
            //const QString two = match.captured(2);   // GPSInfo
            const QString three = match.captured(3); // GPSLongitudeRef
            keys.append(three.toLower() + ",");
            vals.append("'" + i.value() + "',");
        }
    }

    keys.chop(1);
    vals.chop(1);
    queryPrepare.append(keys + ") VALUES(" + vals + ");");

    out << queryPrepare;
    fileOut.flush();
    fileOut.close();
    return std::make_tuple(true, "Rz_writeSQLfile::writeXmpData");
}

std::tuple<bool, std::string> Rz_writeSQLfile::isTargetExist(
    const QFile &pathToTarget,
    const QString &type)
{
    const QFileInfo fInfo(pathToTarget);

    msg = "";
    oknok = false;

    if (type.contains("dir")) {
        if (!pathToTarget.exists()) {
            qDebug() << "createDirectories(fInfo.absolutePath().toStdString(): "
                     << fInfo.absolutePath().toStdString();
            std::tie(oknok, msg) = createDirectories(fInfo.absoluteFilePath().toStdString());
        }
        if (fInfo.isDir() && fInfo.isWritable()) {
            return std::make_tuple(true,
                                   "Rz_writeSQLfile::isTargetExist::dir: yes");
        } else {
            return std::make_tuple(true,
                                   "Rz_writeSQLfile::isTargetExist::dir: no: " +
                                       msg);
        }
    }
    if (!pathToTarget.exists()) {
        return std::make_tuple(false, "Rz_writeSQLfile::isTargetExist:: no");
    }
    if (type.contains("file") && fInfo.isFile() && fInfo.isWritable()) {
        return std::make_tuple(true,
                               "Rz_writeSQLfile::isTargetExist::file: yes");
    }
    return std::make_tuple(false, "Rz_writeSQLfile::isTargetExist:: final no");
}

std::tuple<bool, std::string> Rz_writeSQLfile::createDirectories(
    const std::filesystem::path &p) noexcept
{
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
