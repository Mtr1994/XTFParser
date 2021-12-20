#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Parser/fileparser.h"
#include "Public/common.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void init();

private slots:
    void slot_load_file();
    void slot_export_data();
    void slot_import_data();
    void slot_write_file();

    void slot_thread_bathypack_message(const std::list<XTFBATHHEADER> &listBathyPack);
    void slot_thread_export_bathy_pack_message(const QString &msg);

    void slot_thread_import_bathypack(const std::list<XTFBATHHEADER> &listBathyPack);


private:
    Ui::MainWindow *ui;

    FileParser mFileParser;

    QString mOriginFilePath;

    std::list<XTFBATHHEADER> mListBathyPackParse;

    std::list<XTFBATHHEADER> mListBathyPackImport;
};
#endif // MAINWINDOW_H
