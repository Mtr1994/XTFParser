#include "fileparser.h"
#include "Public/appsignal.h"
#include "Public/common.h"

#include <thread>
#include <QFileInfo>
#include <fstream>

using namespace std;

// test
#include <QDebug>

FileParser::FileParser(QObject *parent) : QObject(parent)
{

}

void FileParser::parse(const QString &filePath)
{
    QFileInfo info(filePath);
    if (!info.exists())
    {
        emit AppSignal::getInstance()->sgl_thread_system_message("文件不存在");
        return;
    }
    if (info.suffix() == "xtf")
    {
        auto func = std::bind(&FileParser::parseXtf, this, filePath);
        std::thread th(func);
        th.detach();
    }
    else
    {
        emit AppSignal::getInstance()->sgl_thread_system_message("不支持的文件类型");
    }
}

void FileParser::parseXtf(const QString &filePath)
{
    // 线程运行
    fstream fileDescripter(filePath.toStdString().data(), ios::in | ios::binary);
    if (!fileDescripter.is_open())
    {
        emit AppSignal::getInstance()->sgl_thread_error_message("文件打开失败: " + filePath);
        return;
    }

    char line[1024] = {0};
    // read head
    fileDescripter.read(line, 1024);

    std::list<XTFBATHHEADER> listBathyPack;

    // data pack
    unsigned short magicNumber = 0;
    fileDescripter.read((char *)&magicNumber, 2);
    while ((magicNumber == 0xface) && (!fileDescripter.eof()))
    {
        fileDescripter.read(line, 12);

        unsigned char headtype = -1;
        memcpy((unsigned char *)&headtype, line, 1);
        uint32_t length  = 0;
        memcpy((char *)&length, line + 8, 4);

        if ((headtype == 61) || (headtype == 0))
        {
            // 读取测深数据
            XTFBATHHEADER bathyHeader;
            // 记录包起始位置
            bathyHeader.PackIndex = fileDescripter.tellg();
            fileDescripter.read(line, 256 - 14);
            fillBathyHeader(line, bathyHeader);
            listBathyPack.emplace_back(bathyHeader);
            fileDescripter.seekg(length - 256, ios::cur);
        }
        else
        {
            fileDescripter.seekg(length - 14, ios::cur);
        }

        fileDescripter.read((char *)&magicNumber, 2);
    }

    emit AppSignal::getInstance()->sgl_thread_bathypack_message(listBathyPack);
}

void FileParser::fillBathyHeader(char *data, XTFBATHHEADER &header)
{
    memcpy(&header.Year, data + 14 - 14, 2);
    memcpy(&header.Month, data + 16 - 14, 1);
    memcpy(&header.Day, data + 17 - 14, 1);
    memcpy(&header.Hour, data + 18 - 14, 1);
    memcpy(&header.Minute, data + 19 - 14, 1);
    memcpy(&header.Second, data + 20 - 14, 1);
    memcpy(&header.HSeconds, data + 21 - 14, 1);
    memcpy(&header.JulianDay, data + 22 - 14, 2);
    memcpy(&header.EventNumber, data + 24 - 14, 4);
    memcpy(&header.PingNumber, data + 28 - 14, 4);
    memcpy(&header.SoundVelocity, data + 32 - 14, 4);
    memcpy(&header.OceanTide, data + 36 - 14, 4);
    memcpy(&header.Reserved2, data + 40 - 14, 4);
    memcpy(&header.ConductivityFreq, data + 44 - 14, 4);
    memcpy(&header.TemperatureFreq, data + 48 - 14, 4);
    memcpy(&header.PressureFreq, data + 52 - 14, 4);
    memcpy(&header.PressureTemp, data + 56 - 14, 4);
    memcpy(&header.Conductivity, data + 60 - 14, 4);
    memcpy(&header.WaterTemperature, data + 64 - 14, 4);
    memcpy(&header.Pressure, data + 68 - 14, 4);
    memcpy(&header.ComputedSoundVelocity, data + 72 - 14, 4);
    memcpy(&header.MagX, data + 76 - 14, 4);
    memcpy(&header.MagY, data + 80 - 14, 4);
    memcpy(&header.MagZ, data + 84 - 14, 4);
    memcpy(&header.AuxVal1, data + 88 - 14, 4);
    memcpy(&header.AuxVal2, data + 92 - 14, 4);
    memcpy(&header.AuxVal3, data + 96 - 14, 4);
    memcpy(&header.AuxVal4, data + 100 - 14, 4);
    memcpy(&header.AuxVal5, data + 104 - 14, 4);
    memcpy(&header.AuxVal6, data + 108 - 14, 4);
    memcpy(&header.SpeedLog, data + 112 - 14, 4);
    memcpy(&header.Turbidity, data + 116 - 14, 4);
    memcpy(&header.ShipSpeed, data + 120 - 14, 4);
    memcpy(&header.ShipGyro, data + 124 - 14, 4);
    memcpy(&header.ShipYcoordinate, data + 128 - 14, 8);
    memcpy(&header.ShipXcoordinate, data + 136 - 14, 8);
    memcpy(&header.ShipAltitude, data + 144 - 14, 2);
    memcpy(&header.ShipDepth, data + 146 - 14, 2);
    memcpy(&header.FixTimeHour, data + 148 - 14, 1);
    memcpy(&header.FixTimeMinute, data + 149 - 14, 1);
    memcpy(&header.FixTimeSecond, data + 150 - 14, 1);
    memcpy(&header.FixTimeHsecond, data + 151 - 14, 1);
    memcpy(&header.SensorSpeed, data + 152 - 14, 4);
    memcpy(&header.KP, data + 156 - 14, 4);
    memcpy(&header.SensorYcoordinate, data + 160 - 14, 8);
    memcpy(&header.SensorXcoordinate, data + 168 - 14, 8);
    memcpy(&header.SonarStatus, data + 176 - 14, 2);
    memcpy(&header.RangeToFish, data + 178 - 14, 2);
    memcpy(&header.BearingToFish, data + 180 - 14, 2);
    memcpy(&header.CableOut, data + 182 - 14, 2);
    memcpy(&header.Layback, data + 184 - 14, 4);
    memcpy(&header.CableTension, data + 188 - 14, 4);
    memcpy(&header.SensorDepth, data + 192 - 14, 4);
    memcpy(&header.SensorPrimaryAltitude, data + 196 - 14, 4);
    memcpy(&header.SensorAuxAltitude, data + 200 - 14, 4);
    memcpy(&header.SensorPitch, data + 204 - 14, 4);
    memcpy(&header.SensorRoll, data + 208 - 14, 4);
    memcpy(&header.SensorHeading, data + 212 - 14, 4);
    memcpy(&header.Heave, data + 216 - 14, 4);
    memcpy(&header.Yaw, data + 220 - 14, 4);
    memcpy(&header.AttitudeTimeTag, data + 224 - 14, 4);
    memcpy(&header.DOT, data + 228 - 14, 4);
    memcpy(&header.NavFixMilliseconds, data + 232 - 14, 4);
    memcpy(&header.ComputerClockHour, data + 236 - 14, 1);
    memcpy(&header.ComputerClockMinute, data + 237 - 14, 1);
    memcpy(&header.ComputerClockSecond, data + 238 - 14, 1);
    memcpy(&header.ComputerClockHsec, data + 239 - 14, 1);
    memcpy(&header.FishPositionDeltaX, data + 240 - 14, 2);
    memcpy(&header.FishPositionDeltaY, data + 242 - 14, 2);
    memcpy(&header.FishPositionErrorCode, data + 244 - 14, 1);
    memcpy(&header.OptionalOffsey, data + 245 - 14, 4);
    memcpy(&header.CableOutHundredths, data + 249 - 14, 1);
    memcpy(header.ReservedSpace2, data + 250 - 14, 6);
}

QString FileParser::getXtfPackName(uint16_t headerType)
{
    switch (headerType) {
    case 0:
        return "XTF_HEADER_SONAR";
    case 1:
        return "XTF_HEADER_NOTES";
    case 2:
        return "XTF_HEADER_BATHY";
    case 3:
        return "XTF_HEADER_ATTITUDE";
    case 4:
        return "XTF_HEADER_FORWARD";
    case 5:
        return "XTF_HEADER_ELAC";
    case 6:
        return "XTF_HEADER_RAW_SERIAL";
    case 7:
        return "XTF_HEADER_EMBED_HEAD";
    case 8:
        return "XTF_HEADER_HIDDEN_SONAR";
    case 9:
        return "XTF_HEADER_SEAVIEW_PROCESSED_BATHY";
    case 10:
        return "XTF_HEADER_SEAVIEW_DEPTHS";
    case 11:
        return "XTF_HEADER_RSVD_HIGHSPEED_SENSOR";
    case 12:
        return "XTF_HEADER_ECHOSTRENGTH";
    case 13:
        return "XTF_HEADER_GEOREC";
    case 14:
        return "XTF_HEADER_KLEIN_RAW_BATHY";
    case 15:
        return "XTF_HEADER_HIGHSPEED_SENSOR2";
    case 16:
        return "XTF_HEADER_ELAC_XSE";
    case 17:
        return "XTF_HEADER_BATHY_XYZA";
    case 18:
        return "XTF_HEADER_K5000_BATHY_IQ";
    case 19:
        return "XTF_HEADER_BATHY_SNIPPET";
    case 20:
        return "XTF_HEADER_GPS";
    case 21:
        return "XTF_HEADER_STAT";
    case 22:
        return "XTF_HEADER_SINGLEBEAM";
    case 23:
        return "XTF_HEADER_GYRO";
    case 24:
        return "XTF_HEADER_TRACKPOINT";
    case 25:
        return "XTF_HEADER_MULTIBEAM";
    case 26:
        return "XTF_HEADER_Q_SINGLEBEAM";
    case 27:
        return "XTF_HEADER_Q_MULTITX";
    case 28:
        return "XTF_HEADER_Q_MULTIBEAM";
    case 50:
        return "XTF_HEADER_TIME";
    case 60:
        return "XTF_HEADER_BENTHOS_CAATI_SARA";
    case 61:
        return "XTF_HEADER_7125";
    case 62:
        return "XTF_HEADER_7125_SNIPPET";
    case 65:
        return "XTF_HEADER_QINSY_R2SONIC_BATHY";
    case 66:
        return "XTF_HEADER_QINSY_R2SONIC_FTS";
    case 67:
    case 68:
        return "XTF_HEADER_R2SONIC_BATHY";
    case 69:
        return "XTF_HEADER_R2SONIC_FTS";
    case 70:
        return "XTF_HEADER_CODA_ECHOSCOPE_DATA";
    case 71:
        return "XTF_HEADER_CODA_ECHOSCOPE_CONFIG";
    case 72:
        return "XTF_HEADER_CODA_ECHOSCOPE_IMAGE";
    case 73:
        return "XTF_HEADER_EDGETECH_4600";
    case 76:
    case 78:
        return "XTF_HEADER_RESON_7018_WATERCOLUMN";
    case 100:
        return "XTF_HEADER_POSITION";
    case 102:
        return "XTF_HEADER_BATHY_PROC";
    case 103:
        return "XTF_HEADER_ATTITUDE_PROC";
    case 104:
        return "XTF_HEADER_SINGLEBEAM_PROC";
    case 105:
        return "XTF_HEADER_AUX_PROC";
    case 106:
        return "XTF_HEADER_KLEIN3000_DATA_PAGE";
    case 107:
        return "XTFPOSRAWNAVIGATION";
    case 108:
        return "XTF_HEADER_KLEINV4_DATA_PAGE";
    case 199:
        return "XTFRAWCUSTOMHEADER";
    case 200:
        return "XTF_HEADER_USERDEFINED";
    default:
        return "NOT REGISTER TYPE";
    }
}
