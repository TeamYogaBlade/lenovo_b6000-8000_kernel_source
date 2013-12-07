/*****************************************************************************
 *
 * Filename:
 * ---------
 *   MT9M114MIPIYUV_Sensor.h
 *
 * Project:
 * --------
 *   ALPS
 *
 * Description:
 * ------------
 *   Header file of Sensor driver
 *
 *
 * Author:
 * -------
 *
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by CC/CQ. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * $Revision:$
 * $Modtime:$
 * $Log:$
 * 
 *
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by CC/CQ. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/
#ifndef __MT9M114MIPIYUV_SENSOR_H
#define __MT9M114MIPIYUV_SENSOR_H

#define MT9M114_WRITE_ID                (0x90)
#define MT9M114_READ_ID                 (0x91)

/* sensor size */
#define MT9M114_IMAGE_SENSOR_FULL_WIDTH         (1280)
#define MT9M114_IMAGE_SENSOR_FULL_HEIGHT        (960)

#define MT9M114_IMAGE_SENSOR_PV_WIDTH           (1280)
#define MT9M114_IMAGE_SENSOR_PV_HEIGHT          (960)

#define MT9M114_IMAGE_SENSOR_VIDEO_WIDTH           (1280)
#define MT9M114_IMAGE_SENSOR_VIDEO_HEIGHT          (960)


#define MT9M114_FULL_START_X                    2
#define MT9M114_FULL_START_Y                    2

#define MT9M114_PV_START_X                      2
#define MT9M114_PV_START_Y                      2


#endif /* __MT9M114MIPIYUV_SENSOR_H */
