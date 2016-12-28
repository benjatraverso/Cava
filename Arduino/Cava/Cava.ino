<<<<<<< HEAD
// Pins
const unsigned int BTN_UP = 0; //DEFINIR!!!
const unsigned int BTN_DOWN = 0; //DEFINIR!!!
// BCDs???

// State Constants
const int STATE_CURRENT = 0;
const int STATE_UP = 1;
const int STATE_DOWN = 2;
const int STATE_TARGET = 3;

// Other Constants
const unsigned long MAX_UL = 4294967295;
const float HOLD_DELAY = 1.0f;
const float TARGET_DELAY = 3.0f;

// Variables
int iState;

bool bUp;
bool bDown;

int iTemp;
int iTarget;

float fInputTimer;
unsigned long oldMicro;
=======
#include<Servo.h>
#include<Wire.h>
>>>>>>> origin/master

enum States
{
<<<<<<< HEAD
	pinMode(BTN_UP, INPUT_PULLUP);
	pinMode(BTN_DOWN, INPUT_PULLUP);
	
	iState = STATE_CURRENT;
	
	bUp = false;
	bDown = false;
	
	iTemp = 0;
	iTemp = 14; //guardar este dato si se apaga el micro???
	
	fInputTimer = 0;
	oldMicro = micros();
=======
	Idle = 0,
	WarmingUp
>>>>>>> origin/master
}

setup()
{
<<<<<<< HEAD
	float dt = getDeltaTime();
	readInput();
	switch (iState)
	{
		case STATE_CURRENT:
			if(bUp && !bDown)
			{
				fInputTimer = 0;
				// update display (switch to target temp)
				state = STATE_UP;
			}
			else if(bDown && !bUp)
			{
				fInputTimer = 0;
				// update display (switch to target temp)
				state = STATE_DOWN;
			}
			break;
		case STATE_UP:
			if(bUp)
			{
				fInputTimer += dt;
				if(fInputTimer >= HOLD_LENGTH)
				{
					iTarget++;
					// check max
					// update display
					fInputTimer -= HOLD_LENGTH;
				}
			}
			else
			{
				fInputTimer = 0;
				iState = STATE_TARGET;
			}
			break;
		case STATE_DOWN:
			if(bDown)
			{
				fInputTimer += dt;
				if(fInputTimer >= HOLD_LENGTH)
				{
					iTarget--;
					// check min
					// update display
					fInputTimer -= HOLD_LENGTH;
				}
			}
			else
			{
				fInputTimer = 0;
				iState = STATE_TARGET;
			}
			break;
		case STATE_TARGET:
			if(bUp && !bDown)
			{
				fInputTimer = 0;
				iTarget++;
				// check max
				// update display
				state = STATE_UP;
			}
			else if(bDown && !bUp)
			{
				fInputTimer = 0;
				iTarget--;
				// check min
				// update display
				state = STATE_DOWN;
			}
			else
			{
				fInputTimer += dt;
				if(fInputTimer >= TARGET_DELAY)
				{
					// update display (switch to current temp)
					iState = STATE_CURRENT;
				}
			}
			break;
	}
}

void readInput()
{
	bUp = LOW == digitalRead(BTN_UP);
	bDown = LOW == digitalRead(BTN_DOWN);
}

float getDeltaTime()
{
	unsigned long newMicro = micros();
	unsigned long ul_dt = 0;
	if(newMicro > oldMicro)
	{
		ul_dt = newMicro - oldMicro;
	}
	else
	{
		ul_dt = MAX_UL - oldMicro + newMicro;
	}
	return dt / 1000000.0f;
=======
	;
>>>>>>> origin/master
}

loop()
{
	;
}

void GetNextStep()
{
	;
}

void DoStep()
{
	;
}