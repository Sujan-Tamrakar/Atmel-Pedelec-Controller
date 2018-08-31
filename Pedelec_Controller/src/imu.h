/*
 * imu.h
 *
 * Created: 6/6/2017 5:17:14 PM
 *  Author: Sujan Tamrakar
 */ 


#ifndef IMU_H_
#define IMU_H_

/*
 * IMU getter
 */
void hmi_get_acelerometer(void);
float acc_raw_2_velocity(void);

#endif /* IMU_H_ */