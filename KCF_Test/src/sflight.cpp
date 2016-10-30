#include <cmath>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>

#include "sflight.hpp"
#define pi 3.1415926

SurroundFlight::SurroundFlight()
{
    m_time = 2.5;
    Cx=120.0,Cy=145.0,Fx=102.0,Fy=57.0;

    current_error=last_error=pid_add=0;
    memset(&m_fc_param, 0x00, sizeof(m_fc_param));
}

SurroundFlight::~SurroundFlight()
{

}

void SurroundFlight::init()
{
    float tmp_velocity = 2; // 初始速度,单位: m/s
    float tmp_time = 0.05;  // 初始时间,单位: s
    float tmp_s = tmp_velocity * tmp_time;

    float v = 100.0; // 目标点的图像坐标(u,v); 实际可通过摄像机获得
    float theta = atan((v-Cy)/Fy);
    m_radius = tmp_s / tan(theta);
    std::fstream out("./out.txt", std::ios::out);
    out << " R: " << m_radius << std::endl;
}

float cur_w = 0;
// 输入一帧图片和一个目标框.计算出飞机的姿态.
void SurroundFlight::update(cv::Mat& finfo, cv::Rect& rect, float timeStamp)
{
    {
        ref = 0;    // 摄像头正对目标
        float v = 100.0; // 可根据摄像机来获取
        feb = perFrameFound;//atan((v-Cy)/Fy);

        myPID(pid_add, last_error, current_error, feb, ref, 0.6, 0.2, 0.0);
        pid_add = pid_add==0.0 ? 1.0f : pid_add;
        std::cout << "pid_add: " << pid_add << std::endl;
        std::cout << " feb : " << feb << std::endl;
	    cur_w += pid_add;
        m_fc_param.x = 50;//(pid_add * 2*pi*m_radius / m_time);
        // 为保证目标在图像像素中心应调节的角度.
        m_fc_param.r = cur_w;
        pre_time = timeStamp;
    }
}

impr_fc_param SurroundFlight::GetSFParam() const
{
    return m_fc_param;
}

void SurroundFlight::myPID(float &pid_add,int &last_error,int &current_error,
            float feb, float ref, float kp, float ki, float kd)
{
    // 误差更新
    int prev_error = last_error;
    last_error = current_error;
    current_error = ref - feb;

    // PID 控制
    float P = kp*(current_error-last_error);
    float D = kd*(current_error-2*last_error+prev_error);
    float I = ki*current_error;

    float pwm_value = P+I+D;
    if(pwm_value>20) pwm_value=10;
    if(pwm_value<-20) pwm_value=-10;

    pid_add = pid_add+pwm_value;
    if(pid_add>20) pid_add=20;
    if(pid_add<-20) pid_add=-20;

}


