#ifndef FILEUTIL_H
#define FILEUTIL_H

#include "Public/common.h"

#include <QObject>

class FileUtil : public QObject
{
    Q_OBJECT
public:
    explicit FileUtil(QObject *parent = nullptr);

    // 导出测深数据
    void exportBathyData(const QString &path, const std::list<XTFBATHHEADER> &listBathyPack);

    // 导入测深数据
    void importBathyData(const QString &path, uint32_t targetSize);

    // 数据写入
    void writeBathyData(const QString &path, const QString &originPath ,const std::list<XTFBATHHEADER> &listBathyPack);

private:
    void exportBathyDataThrowThread(const QString &path, const std::list<XTFBATHHEADER> &listBathyPack);

    void importBathyDataThrowThread(const QString &path, uint32_t targetSize);

    void writeBathyDataThrowThread(const QString &path, const QString &originPath ,const std::list<XTFBATHHEADER> &listBathyPack);

};

#endif // FILEUTIL_H
