#pragma once

#include <QFile>
#include <QFileInfo>
#include <QHash>
#include <QObject>
#include <QRegularExpression>
#include <QtPlugin>
#include <format>

#include "rz_config.hpp"
#include "rz_photo-gallery_plugins.hpp"

class Rz_writeSQLfile : public QObject, public Plugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "net.hase-zheng.photo_gallery_plugins");
    Q_INTERFACES(Plugin);

public:
    explicit Rz_writeSQLfile(QObject *parent = nullptr);
    ~Rz_writeSQLfile();

signals:
public slots:

    // Plugin interface
public:
    QString getPluginNameShort() Q_DECL_OVERRIDE;
    QString getPluginNameLong() Q_DECL_OVERRIDE;
    QString getPluginVersion() Q_DECL_OVERRIDE;
    QString getPluginDescription() Q_DECL_OVERRIDE;

    std::tuple<bool, std::string> parseFile(QMap<QString, QString> &configMap,
                                            QString empty) Q_DECL_OVERRIDE;
    std::tuple<bool, std::string> writeFile(QMap<QString, QString> mapParseKeys,
                                            QMap<QString, QString> mapFileAttribs,
                                            QString pathToFile) Q_DECL_OVERRIDE;
    void setHashMap(const QHash<QString, QString> defaultMetaKeys, QString type) Q_DECL_OVERRIDE;
    QHash<QString, QString> getHashMap(QString type) Q_DECL_OVERRIDE;

    void doClose() Q_DECL_OVERRIDE;

private:
    bool oknok{false};
    std::string msg{"blank"};
    QString debugMsg{"blank"};

    QHash<QString, QString> exifMetaTags;
    QHash<QString, QString> iptcMetaTags;
    QHash<QString, QString> xmpMetaTags;
    void addMetaKeys(const QList<QString> &keys, const QString &type);

    std::tuple<bool, std::string> createSQLfiles(
        const QString pathToSqlFilesFolder = "./SQL-Files");

    std::tuple<bool, std::string> createPhotoTable(
        const QString pathToSqlFilesFolder);
    std::tuple<bool, std::string> createExifTable(
        const QString &pathToSqlFilesFolder);
    std::tuple<bool, std::string> createIptcTable(
        const QString &pathToSqlFilesFolder);
    std::tuple<bool, std::string> createXmpTable(
        const QString &pathToSqlFilesFolder);
    void writeFile(QString &file, QString &content);

    std::tuple<bool, std::string> writeExifData(
        const QString &pathToSqlFilesFolder);
    std::tuple<bool, std::string> writeIptcData(
        const QString &pathToSqlFilesFolder);
    std::tuple<bool, std::string> writeXmpData(
        const QString &pathToSqlFilesFolder);

    std::tuple<bool, std::string> isTargetExist(const QFile &pathToTarget,
                                                const QString &type);
    std::tuple<bool, std::string> createDirectories(const std::filesystem::path &p) noexcept;
};
