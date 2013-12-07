#include <utils/Log.h>
#include <fcntl.h>
#include <math.h>

#include "camera_custom_nvram.h"
#include "camera_custom_sensor.h"
#include "image_sensor.h"
#include "kd_imgsensor_define.h"
#include "camera_AE_PLineTable_ov2722raw.h"
#include "camera_info_ov2722raw.h"
#include "camera_custom_AEPlinetable.h"
const NVRAM_CAMERA_ISP_PARAM_STRUCT CAMERA_ISP_DEFAULT_VALUE =
{{
    //Version
    Version: NVRAM_CAMERA_PARA_FILE_VERSION,
    //SensorId
    SensorId: SENSOR_ID,
    ISPComm:{
        {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        }
    },
    ISPPca:{
        #include INCLUDE_FILENAME_ISP_PCA_PARAM
    },
    ISPRegs:{
        #include INCLUDE_FILENAME_ISP_REGS_PARAM
        },
    ISPMfbMixer:{{
        {//00: MFB mixer for ISO 100
            0x00000000, 0x00000000
        },
        {//01: MFB mixer for ISO 200
            0x00000000, 0x00000000
        },
        {//02: MFB mixer for ISO 400
            0x00000000, 0x00000000
        },
        {//03: MFB mixer for ISO 800
            0x00000000, 0x00000000
        },
        {//04: MFB mixer for ISO 1600
            0x00000000, 0x00000000
        },
        {//05: MFB mixer for ISO 2400
            0x00000000, 0x00000000
        },
        {//06: MFB mixer for ISO 3200
            0x00000000, 0x00000000
        }
    }},
    ISPCcmPoly22:{
        65609,    // i4R_AVG
        13100,    // i4R_STD
        99910,    // i4B_AVG
        26559,    // i4B_STD
        {  // i4P00[9]
            4955000, -1715000, -680000, -897500, 3797500, -340000, -137500, -1835000, 4532500
        },
        {  // i4P10[9]
            545704, -818772, 273068, -140139, 109879, 30260, -193710, 621564, -427854
        },
        {  // i4P01[9]
            335798, -463783, 127984, -174389, 42835, 131555, -154308, -11571, 165879
        },
        {  // i4P20[9]
            0, 0, 0, 0, 0, 0, 0, 0, 0
        },
        {  // i4P11[9]
            0, 0, 0, 0, 0, 0, 0, 0, 0
        },
        {  // i4P02[9]
            0, 0, 0, 0, 0, 0, 0, 0, 0
        }
    }
}};

const NVRAM_CAMERA_3A_STRUCT CAMERA_3A_NVRAM_DEFAULT_VALUE =
{
    NVRAM_CAMERA_3A_FILE_VERSION, // u4Version
    SENSOR_ID, // SensorId

    // AE NVRAM
    {
        // rDevicesInfo
        {
            1152,    // u4MinGain, 1024 base = 1x
            15872,    // u4MaxGain, 16x
            159,    // u4MiniISOGain, ISOxx  
            64,    // u4GainStepUnit, 1x/8 
            39620,    // u4PreExpUnit 
            30,    // u4PreMaxFrameRate
            39620,    // u4VideoExpUnit  
            30,    // u4VideoMaxFrameRate 
            1024,    // u4Video2PreRatio, 1024 base = 1x 
            39620,    // u4CapExpUnit 
            30,    // u4CapMaxFrameRate
            1024,    // u4Cap2PreRatio, 1024 base = 1x
            24,    // u4LensFno, Fno = 2.8
            350    // u4FocusLength_100x
        },
        // rHistConfig
        {
            2,    // u4HistHighThres
            40,    // u4HistLowThres
            2,    // u4MostBrightRatio
            1,    // u4MostDarkRatio
            160,    // u4CentralHighBound
            20,    // u4CentralLowBound
            {240, 230, 220, 210, 200},    // u4OverExpThres[AE_CCT_STRENGTH_NUM] 
            {86, 108, 128, 148, 170},    // u4HistStretchThres[AE_CCT_STRENGTH_NUM] 
            {18, 22, 26, 30, 34}    // u4BlackLightThres[AE_CCT_STRENGTH_NUM] 
        },
        // rCCTConfig
        {
            TRUE,    // bEnableBlackLight
            TRUE,    // bEnableHistStretch
            FALSE,    // bEnableAntiOverExposure
            TRUE,    // bEnableTimeLPF
            FALSE,    // bEnableCaptureThres
            FALSE,    // bEnableVideoThres
            FALSE,    // bEnableStrobeThres
            47,    // u4AETarget
            47,    // u4StrobeAETarget
            50,    // u4InitIndex
            4,    // u4BackLightWeight
            32,    // u4HistStretchWeight
            4,    // u4AntiOverExpWeight
            2,    // u4BlackLightStrengthIndex
            2,    // u4HistStretchStrengthIndex
            2,    // u4AntiOverExpStrengthIndex
            2,    // u4TimeLPFStrengthIndex
            {1, 3, 5, 7, 8},    // u4LPFConvergeTable[AE_CCT_STRENGTH_NUM] 
            90,    // u4InDoorEV = 9.0, 10 base 
            -10,    // i4BVOffset delta BV = value/10 
            4,    // u4PreviewFlareOffset
            4,    // u4CaptureFlareOffset
            5,    // u4CaptureFlareThres
            4,    // u4VideoFlareOffset
            5,    // u4VideoFlareThres
            2,    // u4StrobeFlareOffset
            2,    // u4StrobeFlareThres
            8,    // u4PrvMaxFlareThres
            0,    // u4PrvMinFlareThres
            8,    // u4VideoMaxFlareThres
            0,    // u4VideoMinFlareThres
            18,    // u4FlatnessThres    // 10 base for flatness condition.
            75    // u4FlatnessStrength
        }
    },
    // AWB NVRAM
    {
        // AWB calibration data
        {
            // rUnitGain (unit gain: 1.0 = 512)
            {
                0,    // i4R
                0,    // i4G
                0    // i4B
            },
            // rGoldenGain (golden sample gain: 1.0 = 512)
            {
                0,    // i4R
                0,    // i4G
                0    // i4B
            },
            // rTuningUnitGain (Tuning sample unit gain: 1.0 = 512)
            {
                0,    // i4R
                0,    // i4G
                0    // i4B
            },
            // rD65Gain (D65 WB gain: 1.0 = 512)
            {
                784,    // i4R
                512,    // i4G
                634    // i4B
            }
        },
        // Original XY coordinate of AWB light source
        {
           // Strobe
            {
                79,    // i4X
                -236    // i4Y
            },
            // Horizon
            {
                -463,    // i4X
                -328    // i4Y
            },
            // A
            {
                -344,    // i4X
                -324    // i4Y
            },
            // TL84
            {
                -193,    // i4X
                -311    // i4Y
            },
            // CWF
            {
                -140,    // i4X
                -413    // i4Y
            },
            // DNP
            {
                -55,    // i4X
                -263    // i4Y
            },
            // D65
            {
                79,    // i4X
                -236    // i4Y
            },
            // DF
            {
                0,    // i4X
                0    // i4Y
            }
        },
        // Rotated XY coordinate of AWB light source
        {
            // Strobe
            {
                32,    // i4X
                -247    // i4Y
            },
            // Horizon
            {
                -517,    // i4X
                -233    // i4Y
            },
            // A
            {
                -399,    // i4X
                -252    // i4Y
            },
            // TL84
            {
                -249,    // i4X
                -268    // i4Y
            },
            // CWF
            {
                -216,    // i4X
                -378    // i4Y
            },
            // DNP
            {
                -104,    // i4X
                -247    // i4Y
            },
            // D65
            {
                32,    // i4X
                -247    // i4Y
            },
            // DF
            {
                0,    // i4X
                0    // i4Y
            }
        },
        // AWB gain of AWB light source
        {
            // Strobe 
            {
                784,    // i4R
                512,    // i4G
                634    // i4B
            },
            // Horizon 
            {
                512,    // i4R
                615,    // i4G
                1793    // i4B
            },
            // A 
            {
                512,    // i4R
                526,    // i4G
                1300    // i4B
            },
            // TL84 
            {
                601,    // i4R
                512,    // i4G
                1013    // i4B
            },
            // CWF 
            {
                741,    // i4R
                512,    // i4G
                1084    // i4B
            },
            // DNP 
            {
                678,    // i4R
                512,    // i4G
                788    // i4B
            },
            // D65 
            {
                784,    // i4R
                512,    // i4G
                634    // i4B
            },
            // DF 
            {
                512,    // i4R
                512,    // i4G
                512    // i4B
            }
        },
        // Rotation matrix parameter
        {
            11,    // i4RotationAngle
            251,    // i4Cos
            49    // i4Sin
        },
        // Daylight locus parameter
        {
            -190,    // i4SlopeNumerator
            128    // i4SlopeDenominator
        },
        // AWB light area
        {
            // Strobe:FIXME
            {
            0,    // i4RightBound
            0,    // i4LeftBound
            0,    // i4UpperBound
            0    // i4LowerBound
            },
            // Tungsten
            {
            -299,    // i4RightBound
            -949,    // i4LeftBound
            -192,    // i4UpperBound
            -292    // i4LowerBound
            },
            // Warm fluorescent
            {
            -299,    // i4RightBound
            -949,    // i4LeftBound
            -292,    // i4UpperBound
            -412    // i4LowerBound
            },
            // Fluorescent
            {
            -154,    // i4RightBound
            -299,    // i4LeftBound
            -179,    // i4UpperBound
            -323    // i4LowerBound
            },
            // CWF
            {
            -154,    // i4RightBound
            -299,    // i4LeftBound
            -323,    // i4UpperBound
            -428    // i4LowerBound
            },
            // Daylight
            {
            57,    // i4RightBound
            -154,    // i4LeftBound
            -167,    // i4UpperBound
            -327    // i4LowerBound
            },
            // Shade
            {
            417,    // i4RightBound
            57,    // i4LeftBound
            -167,    // i4UpperBound
            -327    // i4LowerBound
            },
            // Daylight Fluorescent
            {
            0,    // i4RightBound
            0,    // i4LeftBound
            0,    // i4UpperBound
            0    // i4LowerBound
            }
        },
        // PWB light area
        {
            // Reference area
            {
            417,    // i4RightBound
            -949,    // i4LeftBound
            -142,    // i4UpperBound
            -428    // i4LowerBound
            },
            // Daylight
            {
            82,    // i4RightBound
            -154,    // i4LeftBound
            -167,    // i4UpperBound
            -327    // i4LowerBound
            },
            // Cloudy daylight
            {
            207,    // i4RightBound
            7,    // i4LeftBound
            -167,    // i4UpperBound
            -327    // i4LowerBound
            },
            // Shade
            {
            357,    // i4RightBound
            157,    // i4LeftBound
            -167,    // i4UpperBound
            -327    // i4LowerBound
            },
            // Twilight
            {
            -154,    // i4RightBound
            -314,    // i4LeftBound
            -167,    // i4UpperBound
            -327    // i4LowerBound
            },
            // Fluorescent
            {
            82,    // i4RightBound
            -349,    // i4LeftBound
            -197,    // i4UpperBound
            -428    // i4LowerBound
            },
            // Warm fluorescent
            {
            -349,    // i4RightBound
            -499,    // i4LeftBound
            -197,    // i4UpperBound
            -428    // i4LowerBound
            },
            // Incandescent
            {
            -349,    // i4RightBound
            -499,    // i4LeftBound
            -167,    // i4UpperBound
            -327    // i4LowerBound
            },
            // Gray World
            {
            5000,    // i4RightBound
            -5000,    // i4LeftBound
            5000,    // i4UpperBound
            -5000    // i4LowerBound
            }
        },
        // PWB default gain	
        {
            // Daylight
            {
            730,    // i4R
            512,    // i4G
            706    // i4B
            },
            // Cloudy daylight
            {
            850,    // i4R
            512,    // i4G
            563    // i4B
            },
            // Shade
            {
            998,    // i4R
            512,    // i4G
            443    // i4B
            },
            // Twilight
            {
            590,    // i4R
            512,    // i4G
            968    // i4B
            },
            // Fluorescent
            {
            729,    // i4R
            512,    // i4G
            885    // i4B
            },
            // Warm fluorescent
            {
            534,    // i4R
            512,    // i4G
            1404    // i4B
            },
            // Incandescent
            {
            482,    // i4R
            512,    // i4G
            1309    // i4B
            },
            // Gray World
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            }
        },
        // AWB preference color	
        {
            // Tungsten
            {
            50,    // i4SliderValue
            4596    // i4OffsetThr
            },
            // Warm fluorescent	
            {
            50,    // i4SliderValue
            4596    // i4OffsetThr
            },
            // Shade
            {
            50,    // i4SliderValue
            847    // i4OffsetThr
            },
            // Daylight WB gain
            {
            678,    // i4R
            512,    // i4G
            787    // i4B
            },
            // Preference gain: strobe
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: tungsten
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: warm fluorescent
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: fluorescent
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: CWF
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: daylight
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: shade
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: daylight fluorescent
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            }
        },
        {// CCT estimation
            {// CCT
                2300,    // i4CCT[0]
                2850,    // i4CCT[1]
                4100,    // i4CCT[2]
                5100,    // i4CCT[3]
                6500    // i4CCT[4]
            },
            {// Rotated X coordinate
                -549,    // i4RotatedXCoordinate[0]
                -431,    // i4RotatedXCoordinate[1]
                -281,    // i4RotatedXCoordinate[2]
                -136,    // i4RotatedXCoordinate[3]
                0    // i4RotatedXCoordinate[4]
            }
        }
    },
    {0}
};

#include INCLUDE_FILENAME_ISP_LSC_PARAM
//};  //  namespace


typedef NSFeature::RAWSensorInfo<SENSOR_ID> SensorInfoSingleton_T;


namespace NSFeature {
template <>
UINT32
SensorInfoSingleton_T::
impGetDefaultData(CAMERA_DATA_TYPE_ENUM const CameraDataType, VOID*const pDataBuf, UINT32 const size) const
{
    UINT32 dataSize[CAMERA_DATA_TYPE_NUM] = {sizeof(NVRAM_CAMERA_ISP_PARAM_STRUCT),
                                             sizeof(NVRAM_CAMERA_3A_STRUCT),
                                             sizeof(NVRAM_CAMERA_SHADING_STRUCT),
                                             sizeof(NVRAM_LENS_PARA_STRUCT),
                                             sizeof(AE_PLINETABLE_T)};

    if (CameraDataType > CAMERA_DATA_AE_PLINETABLE || NULL == pDataBuf || (size < dataSize[CameraDataType]))
    {
        return 1;
    }

    switch(CameraDataType)
    {
        case CAMERA_NVRAM_DATA_ISP:
            memcpy(pDataBuf,&CAMERA_ISP_DEFAULT_VALUE,sizeof(NVRAM_CAMERA_ISP_PARAM_STRUCT));
            break;
        case CAMERA_NVRAM_DATA_3A:
            memcpy(pDataBuf,&CAMERA_3A_NVRAM_DEFAULT_VALUE,sizeof(NVRAM_CAMERA_3A_STRUCT));
            break;
        case CAMERA_NVRAM_DATA_SHADING:
            memcpy(pDataBuf,&CAMERA_SHADING_DEFAULT_VALUE,sizeof(NVRAM_CAMERA_SHADING_STRUCT));
            break;
        case CAMERA_DATA_AE_PLINETABLE:
            memcpy(pDataBuf,&g_PlineTableMapping,sizeof(AE_PLINETABLE_T));
            break;
        default:
            break;
    }
    return 0;
}}; // NSFeature


