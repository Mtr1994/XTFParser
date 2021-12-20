#ifndef FILEPARSER_H
#define FILEPARSER_H

#include "Public/common.h"

#include <QObject>
#include <QString>

class FileParser : public QObject
{
    Q_OBJECT
public:
    enum DataTypeFlag {XTF_DATA, ALL_DATA, WCD_DATA};
    explicit FileParser(QObject *parent = nullptr);

    void parse(const QString &filePath);

signals:

private:
    void parseXtf(const QString &filePath);
    void fillBathyHeader(char *data, XTFBATHHEADER &header);

    QString getXtfPackName(uint16_t headerType);
};

#endif // FILEPARSER_H
