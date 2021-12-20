#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <stdint.h>

// start XTF 文件格式

typedef struct
{
    unsigned char TypeOfChannel;                                // 0 M SUBBOTTOM=0, PORT=1, STBD=2,
    unsigned char SubChannelNumber;                             // 1 O Index for which CHANINFO structure this is.
    unsigned short CorrectionFlags;                             // 2 O 1=sonar imagery stored as slant-range, 2=sonar
    unsigned short UniPolar;                                    // 4 O 0=data is polar, 1=data is unipolar
    unsigned short BytesPerSample;                              // 6 M 1 (8-bit data) or 2 (16-bit data) or 4 (32-bit)
    unsigned int Reserved;                                      // 8 U Isis Note: Previously this was
    char ChannelName[16];                                       // 12 O Text describing channel. i.e., "Port 500"
    float VoltScale;                                            // 28; O This states how many volts are represented by
    float Frequency;                                            // 32 O Center transmit frequency
    float HorizBeamAngle;                                       // 36 O Typically 1 degree or so
    float TiltAngle;                                            // 40 O Typically 30 degrees
    float BeamWidth;                                            // 44 O 3dB beam width, Typically 50 degrees
    float OffsetX;                                              // 48 O Orientation of positive X is to starboard. Note:
    float OffsetY;                                              // 52 O Orientation of positive Y is forward. Note:
    float OffsetZ;                                              // 56 O Orientation of positive Z is down. Just like
    float OffsetYaw;                                            // 60 O Orientation of positive yaw is turn to right. If
    float OffsetPitch;                                          // 64 O Orientation of positive pitch is nose up. Note:
    float OffsetRoll;                                           // 68 O Orientation of positive roll is lean to starboard.
    unsigned short BeamsPerArray;                               // 72
    unsigned char SampleFormat;                                 // 74
    char ReservedArea2[53];                                     // 75
} CHANINFO;

typedef struct
{
    unsigned char FileFormat;                                   // 0 M Set to 123 (0x7B)
    unsigned char SystemType;                                   // 1 M Set to 1
    char RecordingProgramName[8] ;                              // 2 M Example: "Isis"
    char RecordingProgramVersion[8];                            // 10 M Example: "556" for version 5.56
    char SonarName[16] ;                                        // 18 R Name of server used to access sonar. Example:
    unsigned short SonarType;                                   // 34 M 0 = NONE , default.
    char NoteString[64];                                        // 36 R Notes as entered in the Sonar Setup dialog box
    char ThisFileName[64];                                      // 100 R Name of this file. Example:"LINE12-B.XTF"
    unsigned short NavUnits;                                    // 164 M 0=Meters (i.e., UTM) or 3=Lat/Long
    unsigned short NumberOfSonarChannels;                       // 166 M if > 6, header grows to 2K in size
    unsigned short NumberOfBathymetryChannels;                  // 168 M
    unsigned char NumberOfSnippetChannels;                      // 170 M
    unsigned char NumberOfForwardLookArrays;                    // 171 M
    unsigned short NumberOfEchoStrengthChannels;                // 172 M
    unsigned char NumberOfInterferometryChannels;               // 174 M
    unsigned char Reserved1;                                    // 175 U Reserved. Set to 0.
    unsigned short Reserved2;                                   // 176
    float ReferencePointHeight;                                 // 178 O Height of reference point above water line (m)
    unsigned char ProjectionType[12];                           // 182 U Not currently used. Set to 0.
    unsigned char SpheriodType[10];                             // 194 U Not currently used. Set to 0.
    long NavigationLatency;                                     // 204 O Latency of nav system in milliseconds.
    float OriginY;                                              // 208 U Not currently used. Set to 0.
    float OriginX;                                              // 212 U Not currently used. Set to 0.
    float NavOffsetY;                                           // 216 O Orientation of positive Y is forward. ISIS
    float NavOffsetX;                                           // 220
    float NavOffsetZ;                                           // 224
    float NavOffsetYaw;                                         // 228
    float MRUOffsetY;                                           // 232
    float MRUOffsetX;                                           // 236
    float MRUOffsetZ;                                           // 240
    float MRUOffsetYaw;                                         // 244
    float MRUOffsetPitch;                                       // 248
    float MRUOffsetRoll;                                        // 252
    CHANINFO ChanInfo[6];
} XTFFILEHEADER;

typedef struct
{
    unsigned short MagicNumber;
    unsigned char HeaderType;
    unsigned char SubChannelNumber;
    unsigned short NumChansToFollow;
    unsigned short Reserved1[2];
    unsigned short NumBytesThisRecord;
    unsigned short Year;
    unsigned char Month;                                        // 16 M Fix month.
    unsigned char Day;                                          // 17 M Fix day.
    unsigned char Hour;                                         // 18 M Fix hour.
    unsigned char Minutes;                                      // 19 M Fix minute.
    unsigned char Seconds;                                      // 20 M Fix seconds.
    unsigned short MicroSeconds;                                // 21 M (0 – 9999). Fix tenths of milliseconds.
    double RawYcoordinate;
    double RawXcoordinate;
    double RawAltitude;
    float Pitch;
    float Roll;
    float Heave;
    float Heading;
    unsigned char Reserved2;
} XTFPOSRAWNAVIGATION;

typedef struct
{
//    unsigned short MagicNumber;// 0 M Must be set to 0xFACE (hexadecimal value).
//    unsigned char HeaderType;// 2 M 0 = XTF_HEADER_SONAR (Sidescan data)
//    unsigned char SubChannelNumber;// 3
//    unsigned short NumChansToFollow;//4
//    unsigned short Reserved1[2];//6
//    unsigned int NumBytesThisRecord;// 10 M Total byte count for this ping including this ping
    unsigned short Year;// 14 M Ping year
    unsigned char Month;// 16 M Ping month
    unsigned char Day;// 17 M Ping day
    unsigned char Hour;// 18 M Ping hour
    unsigned char Minute;// 19 M Ping minute
    unsigned char Second;// 20 M Ping seconds
    unsigned char HSeconds;// 21 M Ping hundredths of seconds (0-99)
    unsigned short JulianDay;// 22 O Julian day of a ping’s occurrence.
    unsigned int EventNumber;// 24 O Last logged event number; nav interface template
    unsigned int PingNumber;// 28 M Counts consecutively (usually from 0) and
    float SoundVelocity;// 32 M m/s, Isis uses 750 (one way), some XTF files use
    float OceanTide;// 36 O Altitude above Geoide (from RTK), if present;
    unsigned int Reserved2;// 40 U Unused. Set to 0.
    float ConductivityFreq;// 44 O Conductivity frequency in Hz. nav interface
    float TemperatureFreq;// 48 O Temperature frequency in Hz. nav interface
    float PressureFreq;// 52 O Pressure frequency in Hz. nav interface template
    float PressureTemp;// 56 O Pressure temperature (Degrees C); nav interface
    float Conductivity;// 60 O Conductivity in Siemens/m; nav interface token =
    float WaterTemperature;// 64 O Water temperature in Celsius. nav interface token
    float Pressure;// 68 O Water pressure in psia; nav interface token = {p};
    float ComputedSoundVelocity;// 72 O Meters/second computed from Conductivity,
    float MagX;// 76 O X-axis magnetometer data in mgauss. Nav
    float MagY;// 80 O Y-axis magnetometer data in mgauss. Nav
    float MagZ;// 84 O Z-axis magnetometer data in mgauss. Nav
    float AuxVal1;// 88 O Sensors Information. Nav interface template token
    float AuxVal2;// 92 O Sensors Information. Nav interface template token
    float AuxVal3;// 96 O Sensors Information. Nav interface template token
    float AuxVal4;// 100 O Sensors Information. Nav interface template token
    float AuxVal5;// 104
    float AuxVal6;// 108
    float SpeedLog;// 112
    float Turbidity;// 116
    float ShipSpeed;// 120
    float ShipGyro;// 124
    double ShipYcoordinate;// 128
    double ShipXcoordinate;// 136
    unsigned short ShipAltitude;// 144
    unsigned short ShipDepth;// 146
    unsigned char FixTimeHour;// 148
    unsigned char FixTimeMinute;// 149
    unsigned char FixTimeSecond;// 150
    unsigned char FixTimeHsecond;// 151
    float SensorSpeed;// 152
    float KP;// 156
    double SensorYcoordinate;// 160
    double SensorXcoordinate;// 168
    unsigned short SonarStatus;// 176
    unsigned short RangeToFish;// 178
    unsigned short BearingToFish;// 180
    unsigned short CableOut;// 182
    float Layback;// 184
    float CableTension;// 188
    float SensorDepth;// 192
    float SensorPrimaryAltitude;// 196
    float SensorAuxAltitude;// 200
    float SensorPitch;// 204
    float SensorRoll;// 208
    float SensorHeading;// 212
    float Heave;// 216 O Attitude information. Sensors heave at start of
    float Yaw;// 220 O Attitude information. Sensor yaw. Positive means
    unsigned int AttitudeTimeTag;// 224 R Attitude information. In milliseconds - used to
    float DOT;// 228 O Misc. Distance Off Track
    unsigned int NavFixMilliseconds;// 232 R Misc. millisecond clock value when nav received.
    unsigned char ComputerClockHour;// 236 O Isis Note: The Isis computer clock time when this
    unsigned char ComputerClockMinute;// 237 O Isis Note: see above Isis Note
    unsigned char ComputerClockSecond;// 238 O Isis Note: see above Isis Note
    unsigned char ComputerClockHsec;// 239 O Isis Note: see above Isis Note
    short FishPositionDeltaX;// 240 O Additional Tow Cable and Fish information from
    short FishPositionDeltaY;// 242 O Additional Tow Cable and Fish information from
    unsigned char FishPositionErrorCode; // 244
    unsigned int OptionalOffsey;// 245
    unsigned char CableOutHundredths;// 249
    unsigned char ReservedSpace2[6]; // 250

    // 包的索引位置索引
    unsigned long PackIndex;
} XTFBATHHEADER;

// end XTF 文件格式

#endif // COMMON_H
