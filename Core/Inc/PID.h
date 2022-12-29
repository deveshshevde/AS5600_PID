//def for pid variables

typedef struct PID_cons
{
	float kp ;
	float ki ;
	float kd ;
}PID_cons;


typedef struct PID_var
{
	float rpm_from_as5600;
	float target_rpm;
	uint32_t  time; //for Euler approximation method
	float prev_error;
	int  output;
	float eintegral;
	float pwm1;
}PID_var;

typedef struct MOTOR
{
	float  m1;
}motor;

PID_var PID1(PID_cons con1 , PID_var var1);

float sampletime = 0 ;
float prev_time;

 float  x ;
