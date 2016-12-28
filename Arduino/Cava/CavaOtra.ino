// Pins
const unsigned int BTN_UP = 0; //DEFINIR!!!
const unsigned int BTN_DOWN = 0; //DEFINIR!!!
// BCDs???

// State Constants
enum states
{
	STATE_IDLE = 0,
	STATE_CHANGE
}


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
	DoStep();
	GetNextStep();
}

void GetNextStep( void )
{
	float dt = getDeltaTime();
	updateCurrentTemp(dt);
	readInput();
	switch (iState)
	{
		case STATE_IDLE:
		{
			iState = STATE_CHANGE;
			break;
		}
		case STATE_CHANGE:
		{
			giState = STATE_IDLE;
			break;
		}
	}
}

void DoStep( void )
{
	switch()
	{
		case STATE_CURRENT:
		{
			Idle();
			break;
		}
		case STATE_CHANGE:
		{
			ChangingDesiredTemp();
			break;
		}
		case STATE_TARGET:
		{

			break;
		}
	}
}

void ChangingDesiredTemp( void )
{
	ResetTime();

	while( !OutOfTime() )
	{
		if(gbUp && giTargetTemp < MAX_TEMP )
		{
			gbUp = false;
			giTargetTemp ++;
			ResetTime();
		}

		if(gbDown && giTargetTemp > MIN_TEMP )
		{
			gbDown = false;
			giTargetTemp--;
			ResetTime();
		}

		updateDisplay(giTargetTemp);
	}
}

void Idle( void )
{
	while(!gbUp && !gbDown)
	{
		ControlTemp();
		readInput();
	}
}

void readInput()
{
	gbUp = LOW == digitalRead(BTN_UP);
	gbDown = LOW == digitalRead(BTN_DOWN);
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
