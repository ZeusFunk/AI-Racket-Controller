/*
 * @file: i2c.c
 * @author: mgshawn
 * @Email: mgshawn.t@gmail.com
 * @brief: This file contains the implementation for initializing the I2C interface.
 */


#include "wit_imu.h"

extern volatile char s_cDataUpdate = 0;

// 初始化I2C作为主设备
esp_err_t i2c_master_init(void)
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 400000,
        // .clk_flags = 0,          /*!< Optional, you can use I2C_SCLK_SRC_FLAG_* flags to choose i2c source clock here. */
    };
    esp_err_t err = i2c_param_config(i2c_master_port, &conf);
    if (err != ESP_OK) {
		printf("i2c_param_config ERR\r\n");
        return err;
    }
	printf("i2c_param_config ok\r\n");
	// 安装I2C驱动
    return i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}



// 从I2C设备读取数据
int32_t WitIICRead(uint8_t ucAddr, uint8_t ucReg, uint8_t *p_ucVal, uint32_t uiLen)
{
    // 检查读取长度，为零则直接返回
    if(uiLen == 0)
        return 0;

    // 创建并初始化I2C命令链
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    // 发送设备地址和寄存器地址
    i2c_master_write_byte(cmd, ucAddr, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, ucReg, ACK_CHECK_EN);
    // 重新启动I2C通信
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, ucAddr | 0x01, ACK_CHECK_EN);
    // 读取数据
    for(int i = 0; i < uiLen; i++)
    {
        if(i == uiLen - 1) // 如果是最后一个数据包
            i2c_master_read_byte(cmd, p_ucVal+i, NACK_VAL);
        else
            i2c_master_read_byte(cmd, p_ucVal+i, ACK_VAL);
    }
    i2c_master_stop(cmd);
    // 发送命令并等待完成
    int ret = i2c_master_cmd_begin(i2c_master_port, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

int32_t WitIICWrite(uint8_t ucAddr, uint8_t ucReg, uint8_t *p_ucVal, uint32_t uiLen)
{
    int ret;
    int i;

    if(uiLen == 0)
    	return 0;

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, ucAddr, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, ucReg, ACK_CHECK_EN);
    for(i=0; i<uiLen; i++)
    {
    	if(i == uiLen-1)	// last pack
    		i2c_master_read_byte(cmd, p_ucVal+i, NACK_VAL);
    	else
    		i2c_master_read_byte(cmd, p_ucVal+i, ACK_VAL);
    }
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(i2c_master_port, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}


void Delayms(uint16_t usMs)
{
	vTaskDelay(usMs / portTICK_PERIOD_MS);
}


// 根据传感器寄存器更新数据
void SensorDataUpdata(uint32_t uiReg, uint32_t uiRegNum)
{
	int i;
    for(i = 0; i < uiRegNum; i++)
    {
        switch(uiReg)
        {
//            case AX:
//            case AY:
            case AZ:
				s_cDataUpdate |= ACC_UPDATE;
            break;
//            case GX:
//            case GY:
            case GZ:
				s_cDataUpdate |= GYRO_UPDATE;
            break;
//            case HX:
//            case HY:
            case HZ:
				s_cDataUpdate |= MAG_UPDATE;
            break;
//            case Roll:
//            case Pitch:
            case Yaw:
				s_cDataUpdate |= ANGLE_UPDATE;
            break;
            default:
				s_cDataUpdate |= READ_UPDATE;
			break;
        }
		uiReg++;
    }
}

// 传感器扫描
void AutoScanSensor(void)
{
	int i, iRetry;

	for(i = 0; i < 0x7F; i++)
	{
		WitInit(WIT_PROTOCOL_I2C, i);
		iRetry = 2;
		do
		{
			s_cDataUpdate = 0;
			WitReadReg(AX, 3);
			Delayms(10);
			if(s_cDataUpdate != 0)
			{
				printf("find 0x%02X addr sensor\r\n", i);
				return ;
			}
			iRetry--;
		}while(iRetry);
	}
	printf("can not find sensor\r\n");
	printf("please check your connection\r\n");
}

/**
 * @brief Get the Imu Rawdata object
 * 
 * @note  读取原始加速度、角速度数据	data[0~2] [accelx : accely : accelz]
 * 									  data[3~5] [gyrox : gyroy : gyroz]
 * @param data 
 */
void GetImuRawdata(float* data)
{
	int i;
	WitReadReg(AX, 6);
	Delayms(10);
	if(s_cDataUpdate)
	{
		for(i = 0; i < 3; i++)
		{
			*(data+i) = sReg[AX+i] / 32768.0f * 16.0f;
			*(data+3+i) = sReg[GX+i] / 32768.0f * 2000.0f;
		}
		
		#ifdef DEBUG
		printf("acc:%.3f %.3f %.3f\r\n", data[0], data[1],data[2]);
		printf("gyro:%.2f %.2f %.2f\r\n", data[3], data[4], data[5]);
		#endif
	}
}

/**
 * @brief Get the Imu Quater object
 * 
 * @note  读取四元数据	data[0~3] [q0 : q1 : q2 : q3]
 * @param data 
 */
void GetImuQuater(float * data)
{
	int i;
	WitReadReg(q0, 4);
	Delayms(10);
	for(i = 0; i < 4; i++)
	{
		*(data+i) = sReg[q0+i] / 32768.0f;
	}
	#ifdef DEBUG
	printf("quator:%.2f %.2f %.2f %.2f\r\n",data[0], data[1], data[2], data[3]);
	#endif
}


/**
 * @brief Get the Imu Mag object
 * 
 * @note  读取磁场数据	data[0~2] [magx : magy : magz]
 * @param data 
 */
void GetImuMag(float * data)
{
	int i;
	WitReadReg(HX, 3);
	Delayms(10);
	for(i = 0; i < 3; i++)
	{
		*(data+i) = sReg[HX+i];
	}
	#ifdef DEBUG
	printf("Mag:%.2f %.2f %.2f \r\n",data[0], data[1], data[2]);
	#endif
}

/**
 * @brief Get the Imu Euler object
 * 
 * @note  读取欧拉角数据	data[0~2] [roll : pitch : yaw]
 * @param data 
 */
void GetImuEuler(float * data)
{
	int i;
	WitReadReg(Roll, 3);
	Delayms(10);
	for(i = 0; i < 3; i++)
	{
		*(data+i) = sReg[Roll+i] / 32768.0f*180;
	}
	#ifdef DEBUG
	printf("Mag:%.2f %.2f %.2f \r\n",data[0], data[1], data[2]);
	#endif
}