#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Public/appsignal.h"
#include "Util/fileutil.h"

#include <QScreen>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QStandardPaths>

// test
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    init();

    setWindowTitle("数据处理助手");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    QScreen *screen = QGuiApplication::screens().at(0);
    float width = 640;
    float height = 320;
    if (nullptr != screen)
    {
        QRect rect = screen->availableGeometry();
        width = rect.width() * 0.3 < 640 ? 640 : rect.width() * 0.3;
        height = rect.height() * 0.3 < 320 ? 320 : rect.height() * 0.3;
    }

    resize(width, height);

    qRegisterMetaType<std::list<XTFBATHHEADER>>("std::list<XTFBATHHEADER>");
    connect(AppSignal::getInstance(), &AppSignal::sgl_thread_bathypack_message, this, &MainWindow::slot_thread_bathypack_message, Qt::QueuedConnection);
    connect(AppSignal::getInstance(), &AppSignal::sgl_thread_export_bathy_pack_message, this, &MainWindow::slot_thread_export_bathy_pack_message, Qt::QueuedConnection);
    connect(AppSignal::getInstance(), &AppSignal::sgl_thread_import_bathypack, this, &MainWindow::slot_thread_import_bathypack, Qt::QueuedConnection);
    connect(AppSignal::getInstance(), &AppSignal::sgl_thread_system_message, this, &MainWindow::slot_thread_export_bathy_pack_message, Qt::QueuedConnection);

    connect(ui->btnLoadFile, &QPushButton::clicked, this, &MainWindow::slot_load_file);
    connect(ui->btnExport, &QPushButton::clicked, this, &MainWindow::slot_export_data);
    connect(ui->btnImport, &QPushButton::clicked, this, &MainWindow::slot_import_data);
    connect(ui->btnWriteFile, &QPushButton::clicked, this, &MainWindow::slot_write_file);
}

void MainWindow::slot_load_file()
{
    QString desktop = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString fileName = QFileDialog::getOpenFileName(this, tr("加载文件"), desktop, tr("支持的文件类型 (*.xtf *.all)"));
    if (fileName.isEmpty()) return;

    mFileParser.parse(fileName);
    // 记录原始文件
    mOriginFilePath = fileName;
}

void MainWindow::slot_export_data()
{
    if (mListBathyPackParse.size() == 0)
    {
         ui->lbMsg->setText(QString("无有效测深数据"));
         return;
    }

    QString desktop = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString fileName = QFileDialog::getSaveFileName(this, tr("导出文件"), desktop, tr("测深数据 (*.txt)"));
    if (fileName.isEmpty()) return;

    FileUtil fileUtil;
    fileUtil.exportBathyData(fileName, mListBathyPackParse);
}

void MainWindow::slot_import_data()
{
    QString desktop = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString fileName = QFileDialog::getOpenFileName(this, tr("导入文件"), desktop, tr("测深数据 (*.txt)"));
    if (fileName.isEmpty()) return;

    FileUtil fileUtil;
    fileUtil.importBathyData(fileName, mListBathyPackParse.size());
}

void MainWindow::slot_write_file()
{
    QString desktop = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString fileName = QFileDialog::getSaveFileName(this, tr("导出文件"), desktop, tr("测深数据 (*.xtf)"));
    if (fileName.isEmpty()) return;

    FileUtil fileUtil;
    fileUtil.writeBathyData(fileName, mOriginFilePath, mListBathyPackImport);
}

void MainWindow::slot_thread_bathypack_message(const std::list<XTFBATHHEADER> &listBathyPack)
{
    mListBathyPackParse = listBathyPack;
    ui->lbMsg->setText(QString("测深数据解析完成，数据包总量: %1").arg(mListBathyPackParse.size()));
}

void MainWindow::slot_thread_export_bathy_pack_message(const QString &msg)
{
    ui->lbMsg->setText(msg);
}

void MainWindow::slot_thread_import_bathypack(const std::list<XTFBATHHEADER> &listBathyPack)
{
    mListBathyPackImport = listBathyPack;
    ui->lbMsg->setText("数据导入完成");
}

