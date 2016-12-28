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
const float READ_TEMP_DELAY = 5.0f;

// Variables
int iState;

bool bUp;
bool bDown;

int iTemp;
int iTarget;

bool bDisplayCurrent;

float fInputTimer;
float fReadTempTimer;
unsigned long oldMicro;

void setup()
{
	pinMode(BTN_UP, INPUT_PULLUP);
	pinMode(BTN_DOWN, INPUT_PULLUP);
	
	iState = STATE_CURRENT;
	
	bUp = false;
	bDown = false;
	
	iTemp = 0;
	iTemp = 14; //guardar este dato si se apaga el micro???
	
	bDisplayCurrent = true;
	
	fInputTimer = 0;
	fReadTempTimer = 0;
	oldMicro = micros();
}

void loop()
{
	float dt = getDeltaTime();
	updateCurrentTemp(dt);
	readInput();
	switch (iState)
	{
		case STATE_CURRENT:
			if((bUp && !bDown) || (bDown && !bUp))
			{
				fInputTimer = 0;
				bDisplayCurrent = false;
				updateDisplay();
				if(bUp)
				{
					iState = STATE_UP;
				}
				else
				{
					iState = STATE_DOWN;
				}
			}
			break;
		case STATE_UP:
			if(bUp)
			{
				fInputTimer += dt;
				if(fInputTimer >= HOLD_DELAY)
				{
					iTarget++;
					// check max
					updateDisplay();
					fInputTimer -= HOLD_DELAY;
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
				if(fInputTimer >= HOLD_DELAY)
				{
					iTarget--;
					// check min
					updateDisplay();
					fInputTimer -= HOLD_DELAY;
				}
			}
			else
			{
				fInputTimer = 0;
				iState = STATE_TARGET;
			}
			break;
		case STATE_TARGET:
			if((bUp && !bDown) || (bDown && !bUp))
			{
				if(bUp)
				{
					iTarget++;
					// check max
					iState = STATE_UP;
				}
				else
				{
					iTarget--;
					// check min
					iState = STATE_DOWN;
				}
				fInputTimer = 0;
				updateDisplay();
			}
			else
			{
				fInputTimer += dt;
				if(fInputTimer >= TARGET_DELAY)
				{
					bDisplayCurrent = true;
					updateDisplay();
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

void updateCurrentTemp(float dt)
{
	fReadTempTimer += dt;
	if(fReadTempTimer > READ_TEMP_DELAY)
	{
		fReadTempTimer -= READ_TEMP_DELAY;
		iTemp = readTemperature();
		if(STATE_CURRENT == iState)
		{
			updateDisplay();
		}
	}
}

int readTemperature()
{
	return 0; // DO SOMETHING :)
}

void updateDisplay()
{
	int dec = 0;
	int uni = 0;
	if(bDisplayCurrent)
	{
		dec = iTemp / 10;
		uni = iTemp % 10;
	}
	else
	{
		dec = iTarget / 10;
		uni = iTarget % 10;
	}
	// send dec to BCD 1
	// send uni to BCD 2
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
	return (float)ul_dt / 1000000.0f;
}
