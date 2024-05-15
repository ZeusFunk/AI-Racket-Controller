#ifndef _WIT_IMU_
#define _WIT_IMU_
// _I2C_H_

#include "driver/i2c.h"
#include "wit_c_sdk.h"
#include "driver/uart.h"
#include "REG.h"

#ifdef __cplusplus
extern "C" {
#endif

#define I2C_MASTER_SCL_IO 10                     /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO 11                     /*!< gpio number for I2C master data  */

#define ACK_CHECK_EN 0x1                        /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0x0                       /*!< I2C master will not check ack from slave */
#define ACK_VAL 0x0                             /*!< I2C ack value */
#define NACK_VAL 0x1                            /*!< I2C nack value */
#define I2C_MASTER_TX_BUF_DISABLE 0             /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0             /*!< I2C master doesn't need buffer */
static int i2c_master_port = 0;

#define ACC_UPDATE		0x01
#define GYRO_UPDATE		0x02
#define ANGLE_UPDATE	0x04
#define MAG_UPDATE		0x08
#define QUAT_UPDATE		0x10
#define READ_UPDATE		0x80

// #define DEBUG //调试模式

// typedef struct
// {
//     float f_accel[3];
//     float f_gyro[3];
//     float f_quat[4];
// }imudata_t;//暂时不做使用



esp_err_t i2c_master_init(void);
int32_t WitIICRead(uint8_t ucAddr, uint8_t ucReg, uint8_t *p_ucVal, uint32_t uiLen);
int32_t WitIICWrite(uint8_t ucAddr, uint8_t ucReg, uint8_t *p_ucVal, uint32_t uiLen);
void AutoScanSensor(void);
void SensorDataUpdata(uint32_t uiReg, uint32_t uiRegNum);
void Delayms(uint16_t usMs);
void GetImuRawdata(float* data);
void GetImuQuater(float * data);
void GetImuMag(float * data);
void GetImuEuler(float * data);

#ifdef __cplusplus
}
#endif


#endif