// I/O Pins
const unsigned int BTN_UP = 24;
const unsigned int BTN_DOWN = 23;
const unsigned int BCD_TEN_A = 4;
const unsigned int BCD_TEN_B = 5;
const unsigned int BCD_TEN_C = 6;
const unsigned int BCD_TEN_D = 13;
const unsigned int BCD_UNI_A = 19;
const unsigned int BCD_UNI_B = 18;
const unsigned int BCD_UNI_C = 16;
const unsigned int BCD_UNI_D = 15;

// State Constants
enum EInputState
{
	EIS_IDLE = 0,
	EIS_CHANGE
};

// Other Constants
const unsigned long MAX_UL = 4294967295;
const float HOLD_DELAY = 1.0f;
const float TARGET_DELAY = 3.0f;
const float READ_TEMP_DELAY = 5.0f;
const int TARGET_MAX = 20;
const int TARGET_MIN = 5;

// Variables
EInputState eInputState;

bool bUp;
bool bDown;

int iTemp;
int iTarget;
int iNextTarget;

bool bDisplayCurrent;
bool bUpdateDisplay;

float fReadTempTimer;
float fInputTimer;
unsigned long ulLastTime;

void setup()
{
	setupIO();
	
	eInputState = EIS_IDLE;
  
	bUp = false;
	bDown = false;
	
	iTemp = 0;
	iTarget = 14; //guardar este dato si se apaga el micro???
	iNextTarget = iTarget;
	
	bDisplayCurrent = true;
	bUpdateDisplay = true;
	
	fReadTempTimer = 0;
	fInputTimer = 0;
	ulLastTime = millis();
}

void setupIO()
{
	pinMode(BTN_UP, INPUT_PULLUP);
	pinMode(BTN_DOWN, INPUT_PULLUP);
	
	pinMode(BCD_TEN_A, OUTPUT);
	pinMode(BCD_TEN_B, OUTPUT);
	pinMode(BCD_TEN_C, OUTPUT);
	pinMode(BCD_TEN_D, OUTPUT);
	pinMode(BCD_UNI_A, OUTPUT);
	pinMode(BCD_UNI_B, OUTPUT);
	pinMode(BCD_UNI_C, OUTPUT);
	pinMode(BCD_UNI_D, OUTPUT);
	
	digitalWrite(BCD_TEN_A, LOW);
	digitalWrite(BCD_TEN_B, LOW);
	digitalWrite(BCD_TEN_C, LOW);
	digitalWrite(BCD_TEN_D, LOW);
	digitalWrite(BCD_UNI_A, LOW);
	digitalWrite(BCD_UNI_B, LOW);
	digitalWrite(BCD_UNI_C, LOW);
	digitalWrite(BCD_UNI_D, LOW);
}

void loop()
{
	float dt = getDeltaTime();
	loopTempControl(dt);
	loopUserInput(dt);
	loopDisplay();
}

/////////////////////////
// TEMPERATURE CONTROL //
/////////////////////////

void loopTempControl(float dt)
{
	fReadTempTimer += dt;
	if(fReadTempTimer > READ_TEMP_DELAY)
	{
		fReadTempTimer = 0;
		iTemp = readTemperature();
		if(bDisplayCurrent)
		{
			bUpdateDisplay = true;
		}
		
		// Compare temperature with target and do something about it!!!
	}
}

int readTemperature()
{
	return 0; // DO SOMETHING :)
}

///////////////////////
// HANDLE USER INPUT //
///////////////////////

void loopUserInput(float dt)
{
	readInput();
	switch (eInputState)
	{
		case EIS_IDLE:
			stateUserInputIdle(dt);
			break;
		case EIS_CHANGE:
			stateUserInputChange(dt);
			break;
	}
}

void readInput()
{
	bUp = LOW == digitalRead(BTN_UP);
	bDown = LOW == digitalRead(BTN_DOWN);
}

void stateUserInputIdle(float dt)
{
	if(bUp || bDown)
	{
		fInputTimer = 0;
		eInputState = EIS_CHANGE;
		if(bDisplayCurrent)
		{
			bDisplayCurrent = false;
		}
		else
		{
			if(bUp)
			{
				iNextTarget = min(iTarget + 1, TARGET_MAX);
			}
			else
			{
				iNextTarget = max(iTarget - 1, TARGET_MIN);
			}
		}
		bUpdateDisplay = true;
	}
	else
	{
		if(!bDisplayCurrent)
		{
			fInputTimer += dt;
			if(fInputTimer >= TARGET_DELAY)
			{
				iTarget = iNextTarget;
				bDisplayCurrent = true;
				bUpdateDisplay = true;
			}
		}
	}
}

void stateUserInputChange(float dt)
{
	if(bUp || bDown)
	{
		fInputTimer += dt;
		if(fInputTimer >= HOLD_DELAY)
		{
			if(bUp)
			{
				iNextTarget = min(iTarget + 1, TARGET_MAX);
			}
			else
			{
				iNextTarget = max(iTarget - 1, TARGET_MIN);
			}
			bUpdateDisplay = true;
			fInputTimer = 0;
		}
	}
	else
	{
		fInputTimer = 0;
		eInputState = EIS_IDLE;
	}
}

/////////////
// DISPLAY //
/////////////

void loopDisplay()
{
	if(bUpdateDisplay)
	{
		if(bDisplayCurrent)
		{
			setDisplay(iTemp);
		}
		else
		{
			setDisplay(iTarget);
		}
	}
}

void setDisplay(int num)
{
	int ten = 0;
	int uni = 0;
	if(num > 25)
	{
		ten = 11;
		uni = 11;
	}
	else if(num < 0)
	{
		ten = 12;
		uni = 12;
	}
	else
	{
		ten = num / 10;
		if(0 == ten)
		{
			ten = 15;
		}
		uni = num % 10;
	}
	outputIntToBcd(ten, BCD_TEN_A, BCD_TEN_B, BCD_TEN_C, BCD_TEN_D);
	outputIntToBcd(uni, BCD_UNI_A, BCD_UNI_B, BCD_UNI_C, BCD_UNI_D);
}

void setDisplay(char c1, char c2)
{
	outputIntToBcd((int)c1, BCD_TEN_A, BCD_TEN_B, BCD_TEN_C, BCD_TEN_D);
	outputIntToBcd((int)c2, BCD_UNI_A, BCD_UNI_B, BCD_UNI_C, BCD_UNI_D);
}

void outputIntToBcd(int digit, int pinA, int pinB, int pinC, int pinD)
{
	digitalWrite(pinA, 1 == bitRead(digit, 0) ? HIGH : LOW);
	digitalWrite(pinB, 1 == bitRead(digit, 1) ? HIGH : LOW);
	digitalWrite(pinC, 1 == bitRead(digit, 2) ? HIGH : LOW);
	digitalWrite(pinD, 1 == bitRead(digit, 3) ? HIGH : LOW);
}

///////////
// UTILS //
///////////

float getDeltaTime()
{
	unsigned long ulNewTime = millis();
	unsigned long ul_dt = 0;
	if(ulNewTime > ulLastTime)
	{
		ul_dt = ulNewTime - ulLastTime;
	}
	else
	{
		ul_dt = MAX_UL - ulLastTime + ulNewTime;
	}
	return (float)ul_dt / 1000.0f;
}
