#ifndef _SURROUND_FLIGHT_H_
#define _SURROUND_FLIGHT_H_

#include<opencv2/core/core.hpp>

struct impr_fc_param
{
    unsigned int x;
    unsigned int y;
    unsigned int z;
    unsigned int r;
};

class SurroundFlight
{
    public:
        SurroundFlight();
        ~SurroundFlight();
        void init();
        impr_fc_param GetSFParam() const;
        void update(cv::Mat& frame, cv::Rect& rect, float time);

    private:
        float pre_time;
        float m_radius, m_time;
        float ref, feb, pid_add;
        int last_error, current_error;

        float Cx, Cy, Fx, Fy;
        impr_fc_param  m_fc_param;

        void myPID(float &pid_add,int &last_error,int &current_error,
            float feb, float ref, float kp, float ki, float kd);
};

#endif
