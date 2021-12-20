#include "fileutil.h"
#include "Public/appsignal.h"

#include <thread>
#include <fstream>
#include <QStringList>
#include <QFile>

using namespace std;

// test
#include <QDebug>

FileUtil::FileUtil(QObject *parent) : QObject(parent)
{

}

void FileUtil::exportBathyData(const QString &path, const std::list<XTFBATHHEADER> &listBathyPack)
{
    auto func = std::bind(&FileUtil::exportBathyDataThrowThread, this, path, listBathyPack);
    std::thread th(func);
    th.detach();
}

void FileUtil::importBathyData(const QString &path, uint32_t targetSize)
{
    auto func = std::bind(&FileUtil::importBathyDataThrowThread, this, path, targetSize);
    std::thread th(func);
    th.detach();
}

void FileUtil::writeBathyData(const QString &path, const QString &originPath, const std::list<XTFBATHHEADER> &listBathyPack)
{
    auto func = std::bind(&FileUtil::writeBathyDataThrowThread, this, path, originPath, listBathyPack);
    std::thread th(func);
    th.detach();
}

void FileUtil::exportBathyDataThrowThread(const QString &path, const std::list<XTFBATHHEADER> &listBathyPack)
{
    if (listBathyPack.size() == 0)
    {
        emit AppSignal::getInstance()->sgl_thread_export_bathy_pack_message("数据导出完成");
        return;
    }

    ofstream fileDescripter(path.toStdString());
    if (!fileDescripter.is_open())
    {
        emit AppSignal::getInstance()->sgl_thread_export_bathy_pack_message(QString("无法打开导出文件 %1").arg(path));
        return;
    }

    for (auto &pack : listBathyPack)
    {
        fileDescripter << QString::number(pack.Year).toStdString() << ",";// 14 M Ping year
        fileDescripter << QString::number(pack.Month).toStdString() << ",";// 16 M Ping month
        fileDescripter << QString::number(pack.Day).toStdString() << ",";// 17 M Ping day
        fileDescripter << QString::number(pack.Hour).toStdString() << ",";// 18 M Ping hour
        fileDescripter << QString::number(pack.Minute).toStdString() << ",";// 19 M Ping minute
        fileDescripter << QString::number(pack.Second).toStdString() << ",";// 20 M Ping seconds
        fileDescripter << QString::number(pack.HSeconds).toStdString() << ",";// 21 M Ping hundredths of seconds (0-99)
        fileDescripter << QString::number(pack.JulianDay).toStdString() << ",";// 22 O Julian day of a ping’s occurrence.
        fileDescripter << QString::number(pack.EventNumber).toStdString() << ",";// 24 O Last logged event number; nav interface template
        fileDescripter << QString::number(pack.PingNumber).toStdString() << ",";// 28 M Counts consecutively (usually from 0) and
        fileDescripter << QString::number(pack.SoundVelocity, 'f', 4).toStdString() << ",";// 32 M m/s, Isis uses 750 (one way), some XTF files use
        fileDescripter << QString::number(pack.OceanTide, 'f', 4).toStdString() << ",";// 36 O Altitude above Geoide (from RTK), if present;
        fileDescripter << QString::number( pack.Reserved2).toStdString() << ",";// 40 U Unused. Set to 0.
        fileDescripter << QString::number(pack.ConductivityFreq, 'f', 4).toStdString() << ",";// 44 O Conductivity frequency in Hz. nav interface
        fileDescripter << QString::number(pack.TemperatureFreq, 'f', 4).toStdString() << ",";// 48 O Temperature frequency in Hz. nav interface
        fileDescripter << QString::number(pack.PressureFreq, 'f', 4).toStdString() << ",";// 52 O Pressure frequency in Hz. nav interface template
        fileDescripter << QString::number(pack.PressureTemp, 'f', 4).toStdString() << ",";// 56 O Pressure temperature (Degrees C); nav interface
        fileDescripter << QString::number(pack.Conductivity, 'f', 4).toStdString() << ",";// 60 O Conductivity in Siemens/m; nav interface token =
        fileDescripter << QString::number(pack.WaterTemperature, 'f', 4).toStdString() << ",";// 64 O Water temperature in Celsius. nav interface token
        fileDescripter << QString::number(pack.Pressure, 'f', 4).toStdString() << ",";// 68 O Water pressure in psia; nav interface token = {p};
        fileDescripter << QString::number(pack.ComputedSoundVelocity, 'f', 4).toStdString() << ",";// 72 O Meters/second computed from Conductivity,
        fileDescripter << QString::number(pack.MagX, 'f', 4).toStdString() << ",";// 76 O X-axis magnetometer data in mgauss. Nav
        fileDescripter << QString::number(pack.MagY, 'f', 4).toStdString() << ",";// 80 O Y-axis magnetometer data in mgauss. Nav
        fileDescripter << QString::number(pack.MagZ, 'f', 4).toStdString() << ",";// 84 O Z-axis magnetometer data in mgauss. Nav
        fileDescripter << QString::number(pack.AuxVal1, 'f', 4).toStdString() << ",";// 88 O Sensors Information. Nav interface template token
        fileDescripter << QString::number(pack.AuxVal2, 'f', 4).toStdString() << ",";// 92 O Sensors Information. Nav interface template token
        fileDescripter << QString::number(pack.AuxVal3, 'f', 4).toStdString() << ",";// 96 O Sensors Information. Nav interface template token
        fileDescripter << QString::number(pack.AuxVal4, 'f', 4).toStdString() << ",";// 100 O Sensors Information. Nav interface template token
        fileDescripter << QString::number(pack.AuxVal5, 'f', 4).toStdString() << ",";// 104
        fileDescripter << QString::number(pack.AuxVal6, 'f', 4).toStdString() << ",";// 108
        fileDescripter << QString::number(pack.SpeedLog, 'f', 4).toStdString() << ",";// 112
        fileDescripter << QString::number(pack.Turbidity, 'f', 4).toStdString() << ",";// 116
        fileDescripter << QString::number(pack.ShipSpeed, 'f', 4).toStdString() << ",";// 120
        fileDescripter << QString::number(pack.ShipGyro, 'f', 4).toStdString() << ",";// 124
        fileDescripter << QString::number(pack.ShipYcoordinate, 'f', 8).toStdString() << ",";// 128
        fileDescripter << QString::number(pack.ShipXcoordinate, 'f', 8).toStdString() << ",";// 136
        fileDescripter << QString::number(pack.ShipAltitude).toStdString() << ",";// 144
        fileDescripter << QString::number(pack.ShipDepth).toStdString() << ",";// 146
        fileDescripter << QString::number(pack.FixTimeHour).toStdString() << ",";// 148
        fileDescripter << QString::number(pack.FixTimeMinute).toStdString() << ",";// 149
        fileDescripter << QString::number(pack.FixTimeSecond).toStdString() << ",";// 150
        fileDescripter << QString::number(pack.FixTimeHsecond).toStdString() << ",";// 151
        fileDescripter << QString::number(pack.SensorSpeed, 'f', 4).toStdString() << ",";// 152
        fileDescripter << QString::number(pack.KP, 'f', 4).toStdString() << ",";// 156
        fileDescripter << QString::number(pack.SensorYcoordinate, 'f', 8).toStdString() << ",";// 160
        fileDescripter << QString::number(pack.SensorXcoordinate, 'f', 8).toStdString() << ",";// 168
        fileDescripter << QString::number(pack.SonarStatus).toStdString() << ",";// 176
        fileDescripter << QString::number(pack.RangeToFish).toStdString() << ",";// 178
        fileDescripter << QString::number(pack.BearingToFish).toStdString() << ",";// 180
        fileDescripter << QString::number(pack.CableOut).toStdString() << ",";// 182
        fileDescripter << QString::number(pack.Layback, 'f', 4).toStdString() << ",";// 184
        fileDescripter << QString::number(pack.CableTension, 'f', 4).toStdString() << ",";// 188
        fileDescripter << QString::number(pack.SensorDepth, 'f', 4).toStdString() << ",";// 192
        fileDescripter << QString::number(pack.SensorPrimaryAltitude, 'f', 4).toStdString() << ",";// 196
        fileDescripter << QString::number(pack.SensorAuxAltitude, 'f', 4).toStdString() << ",";// 200
        fileDescripter << QString::number(pack.SensorPitch, 'f', 4).toStdString() << ",";// 204
        fileDescripter << QString::number(pack.SensorRoll, 'f', 4).toStdString() << ",";// 208
        fileDescripter << QString::number(pack.SensorHeading, 'f', 4).toStdString() << ",";// 212
        fileDescripter << QString::number(pack.Heave, 'f', 4).toStdString() << ",";// 216 O Attitude information. Sensors heave at start of
        fileDescripter << QString::number(pack.Yaw, 'f', 4).toStdString() << ",";// 220 O Attitude information. Sensor yaw. Positive means
        fileDescripter << QString::number(pack.AttitudeTimeTag).toStdString() << ",";// 224 R Attitude information. In milliseconds - used to
        fileDescripter << QString::number(pack.DOT, 'f', 4).toStdString() << ",";// 228 O Misc. Distance Off Track
        fileDescripter << QString::number(pack.NavFixMilliseconds).toStdString() << ",";// 232 R Misc. millisecond clock value when nav received.
        fileDescripter << QString::number(pack.ComputerClockHour).toStdString() << ",";// 236 O Isis Note: The Isis computer clock time when this
        fileDescripter << QString::number(pack.ComputerClockMinute).toStdString() << ",";// 237 O Isis Note: see above Isis Note
        fileDescripter << QString::number(pack.ComputerClockSecond).toStdString() << ",";// 238 O Isis Note: see above Isis Note
        fileDescripter << QString::number(pack.ComputerClockHsec).toStdString() << ",";// 239 O Isis Note: see above Isis Note
        fileDescripter << QString::number(pack.FishPositionDeltaX).toStdString() << ",";// 240 O Additional Tow Cable and Fish information from
        fileDescripter << QString::number(pack.FishPositionDeltaY).toStdString() << ",";// 242 O Additional Tow Cable and Fish information from
        fileDescripter << QString::number(pack.FishPositionErrorCode).toStdString() << ","; // 244
        fileDescripter << QString::number(pack.OptionalOffsey).toStdString() << ",";// 245
        fileDescripter << QString::number(pack.CableOutHundredths).toStdString() << ",";// 249

        std::string reserve((char *)pack.ReservedSpace2);
        fileDescripter << QString::fromStdString(reserve).toStdString() << ",";
        fileDescripter << QString::number(pack.PackIndex).toStdString() << "\n";
    }

    emit AppSignal::getInstance()->sgl_thread_export_bathy_pack_message(QString("导出文件完成 %1").arg(path));
}

void FileUtil::importBathyDataThrowThread(const QString &path, uint32_t targetSize)
{
    ifstream fileDescripter(path.toStdString());
    if (!fileDescripter.is_open())
    {
        emit AppSignal::getInstance()->sgl_thread_export_bathy_pack_message(QString("无法打开导入文件 %1").arg(path));
        return;
    }

    std::list<XTFBATHHEADER> listBathyPack;

    std::string line;
    while (getline(fileDescripter, line))
    {
        QStringList list = QString::fromStdString(line).split(',');
        if (list.size() != 74)
        {
            emit AppSignal::getInstance()->sgl_thread_export_bathy_pack_message(QString("数据异常，在 %1 行").arg(QString::number(listBathyPack.size())));
            return;
        }
        XTFBATHHEADER header
        {
            list.at(0).toUShort(),
            (unsigned char)list.at(1).toUShort(),
            (unsigned char)list.at(2).toUShort(),
            (unsigned char)list.at(3).toUShort(),
            (unsigned char)list.at(4).toUShort(),
            (unsigned char)list.at(5).toUShort(),
            (unsigned char)list.at(6).toUShort(),
            list.at(7).toUShort(),
            list.at(8).toUInt(),
            list.at(9).toUInt(),
            list.at(10).toFloat(),
            list.at(11).toFloat(),
            list.at(12).toUInt(),
            list.at(13).toFloat(),
            list.at(14).toFloat(),
            list.at(15).toFloat(),
            list.at(16).toFloat(),
            list.at(17).toFloat(),
            list.at(18).toFloat(),
            list.at(19).toFloat(),
            list.at(20).toFloat(),
            list.at(21).toFloat(),
            list.at(22).toFloat(),
            list.at(23).toFloat(),
            list.at(24).toFloat(),
            list.at(25).toFloat(),
            list.at(26).toFloat(),
            list.at(27).toFloat(),
            list.at(28).toFloat(),
            list.at(29).toFloat(),
            list.at(30).toFloat(),
            list.at(31).toFloat(),
            list.at(32).toFloat(),
            list.at(33).toFloat(),
            list.at(34).toDouble(),
            list.at(35).toDouble(),
            list.at(36).toUShort(),
            list.at(37).toUShort(),
            (unsigned char)list.at(38).toUShort(),
            (unsigned char)list.at(39).toUShort(),
            (unsigned char)list.at(40).toUShort(),
            (unsigned char)list.at(41).toUShort(),
            list.at(42).toFloat(),
            list.at(43).toFloat(),
            list.at(44).toDouble(),
            list.at(45).toDouble(),
            list.at(46).toUShort(),
            list.at(47).toUShort(),
            list.at(48).toUShort(),
            list.at(49).toUShort(),
            list.at(50).toFloat(),
            list.at(51).toFloat(),
            list.at(52).toFloat(),
            list.at(53).toFloat(),
            list.at(54).toFloat(),
            list.at(55).toFloat(),
            list.at(56).toFloat(),
            list.at(57).toFloat(),
            list.at(58).toFloat(),
            list.at(59).toFloat(),
            list.at(60).toUInt(),
            list.at(61).toFloat(),
            list.at(62).toUInt(),
            (unsigned char)list.at(63).toUShort(),
            (unsigned char)list.at(64).toUShort(),
            (unsigned char)list.at(65).toUShort(),
            (unsigned char)list.at(66).toUShort(),
            list.at(67).toShort(),
            list.at(68).toShort(),
            (unsigned char)list.at(69).toUShort(),
            list.at(70).toUInt(),
            (unsigned char)list.at(71).toUShort(),
            "",
            list.at(73).toULong(),
        };
        // 写入最后 6 个字节
        QString reservedSpace2 = list.at(72).rightRef(6).toString();
        memcpy(header.ReservedSpace2, &reservedSpace2, 6);
        listBathyPack.emplace_back(header);
    }

    if (listBathyPack.size() != targetSize)
    {
        emit AppSignal::getInstance()->sgl_thread_export_bathy_pack_message(QString("导入文件与目标输入不符,目标 {%1} 导入 {%2}").arg(QString::number(targetSize), QString::number(listBathyPack.size())));
    }
    else
    {
        emit AppSignal::getInstance()->sgl_thread_import_bathypack(listBathyPack);
    }
}

void FileUtil::writeBathyDataThrowThread(const QString &path, const QString &originPath, const std::list<XTFBATHHEADER> &listBathyPack)
{
    bool status = QFile::copy(originPath, path);
    if (!status)
    {
        emit AppSignal::getInstance()->sgl_thread_system_message(QString("文件拷贝失败 %1").arg(originPath));
        return;
    }

    ofstream fileDescripter(path.toStdString(), ios::binary | ios::in | ios::out);
    if (!fileDescripter.is_open())
    {
        emit AppSignal::getInstance()->sgl_thread_system_message(QString("文件打开失败 %1").arg(path));
        return;
    }

    for (auto &pack : listBathyPack)
    {
        fileDescripter.seekp(pack.PackIndex);
        fileDescripter.write((char *)&pack.Year, 2);
        fileDescripter.write((char *)&pack.Month, 1);
        fileDescripter.write((char *)&pack.Day, 1);
        fileDescripter.write((char *)&pack.Hour, 1);
        fileDescripter.write((char *)&pack.Minute, 1);
        fileDescripter.write((char *)&pack.Second, 1);
        fileDescripter.write((char *)&pack.HSeconds, 1);
        fileDescripter.write((char *)&pack.JulianDay, 2);
        fileDescripter.write((char *)&pack.EventNumber, 4);
        fileDescripter.write((char *)&pack.PingNumber, 4);
        fileDescripter.write((char *)&pack.SoundVelocity, 4);
        fileDescripter.write((char *)&pack.OceanTide, 4);
        fileDescripter.write((char *)&pack.Reserved2, 4);
        fileDescripter.write((char *)&pack.ConductivityFreq, 4);
        fileDescripter.write((char *)&pack.TemperatureFreq, 4);
        fileDescripter.write((char *)&pack.PressureFreq, 4);
        fileDescripter.write((char *)&pack.PressureTemp, 4);
        fileDescripter.write((char *)&pack.Conductivity, 4);
        fileDescripter.write((char *)&pack.WaterTemperature, 4);
        fileDescripter.write((char *)&pack.Pressure, 4);
        fileDescripter.write((char *)&pack.ComputedSoundVelocity, 4);
        fileDescripter.write((char *)&pack.MagX, 4);
        fileDescripter.write((char *)&pack.MagY, 4);
        fileDescripter.write((char *)&pack.MagZ, 4);
        fileDescripter.write((char *)&pack.AuxVal1, 4);
        fileDescripter.write((char *)&pack.AuxVal2, 4);
        fileDescripter.write((char *)&pack.AuxVal3, 4);
        fileDescripter.write((char *)&pack.AuxVal4, 4);
        fileDescripter.write((char *)&pack.AuxVal5, 4);
        fileDescripter.write((char *)&pack.AuxVal6, 4);
        fileDescripter.write((char *)&pack.SpeedLog, 4);
        fileDescripter.write((char *)&pack.Turbidity, 4);
        fileDescripter.write((char *)&pack.ShipSpeed, 4);
        fileDescripter.write((char *)&pack.ShipGyro, 4);
        fileDescripter.write((char *)&pack.ShipYcoordinate, 8);
        fileDescripter.write((char *)&pack.ShipXcoordinate, 8);
        fileDescripter.write((char *)&pack.ShipAltitude, 2);
        fileDescripter.write((char *)&pack.ShipDepth, 2);
        fileDescripter.write((char *)&pack.FixTimeHour, 1);
        fileDescripter.write((char *)&pack.FixTimeMinute, 1);
        fileDescripter.write((char *)&pack.FixTimeSecond, 1);
        fileDescripter.write((char *)&pack.FixTimeHsecond, 1);
        fileDescripter.write((char *)&pack.SensorSpeed, 4);
        fileDescripter.write((char *)&pack.KP, 4);
        fileDescripter.write((char *)&pack.SensorYcoordinate, 8);
        fileDescripter.write((char *)&pack.SensorXcoordinate, 8);
        fileDescripter.write((char *)&pack.SonarStatus, 2);
        fileDescripter.write((char *)&pack.RangeToFish, 2);
        fileDescripter.write((char *)&pack.BearingToFish, 2);
        fileDescripter.write((char *)&pack.CableOut, 2);
        fileDescripter.write((char *)&pack.Layback, 4);
        fileDescripter.write((char *)&pack.CableTension, 4);
        fileDescripter.write((char *)&pack.SensorDepth, 4);
        fileDescripter.write((char *)&pack.SensorPrimaryAltitude, 4);
        fileDescripter.write((char *)&pack.SensorAuxAltitude, 4);
        fileDescripter.write((char *)&pack.SensorPitch, 4);
        fileDescripter.write((char *)&pack.SensorRoll, 4);
        fileDescripter.write((char *)&pack.SensorHeading, 4);
        fileDescripter.write((char *)&pack.Heave, 4);
        fileDescripter.write((char *)&pack.Yaw, 4);
        fileDescripter.write((char *)&pack.AttitudeTimeTag, 4);
        fileDescripter.write((char *)&pack.DOT, 4);
        fileDescripter.write((char *)&pack.NavFixMilliseconds, 4);
        fileDescripter.write((char *)&pack.ComputerClockHour, 1);
        fileDescripter.write((char *)&pack.ComputerClockMinute, 1);
        fileDescripter.write((char *)&pack.ComputerClockSecond, 1);
        fileDescripter.write((char *)&pack.ComputerClockHsec, 1);
        fileDescripter.write((char *)&pack.FishPositionDeltaX, 2);
        fileDescripter.write((char *)&pack.FishPositionDeltaY, 2);
        fileDescripter.write((char *)&pack.FishPositionErrorCode, 1);
        fileDescripter.write((char *)&pack.OptionalOffsey, 4);
        fileDescripter.write((char *)&pack.CableOutHundredths, 1);
        fileDescripter.write((char *)pack.ReservedSpace2, 6);
    }

    emit AppSignal::getInstance()->sgl_thread_system_message(QString("数据写入完成 %1").arg(path));
}
