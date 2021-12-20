#ifndef APPSIGNAL_H
#define APPSIGNAL_H

#include "Public/common.h"

#include <QObject>
#include <QString>
#include <QStringList>
#include <tuple>

class vtkActor;
class AppSignal : public QObject
{
    Q_OBJECT
private:
    explicit AppSignal(QObject *parent = nullptr);
    AppSignal(const AppSignal &appsignal) = delete;
    AppSignal &operator=(const AppSignal &appsignal) = delete;

public:
    static AppSignal* getInstance();

signals:
    // 系统消息
    void sgl_thread_system_message(const QString& msg);
    void sgl_thread_error_message(const QString& msg);

    // 解析后的测深文件信息包
    void sgl_thread_bathypack_message(const std::list<XTFBATHHEADER> &listBathyPack);

    // 侧身数据导出消息
    void sgl_thread_export_bathy_pack_message(const QString &msg);

    // 导入测深文件信息包
    void sgl_thread_import_bathypack(const std::list<XTFBATHHEADER> &listBathyPack);
};

#endif // APPSIGNAL_H
