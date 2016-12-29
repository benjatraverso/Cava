// Pins
// DEFINIR TODO!!!!!!!!!!!!!!!
// DEFINIR TODO!!!!!!!!!!!!!!!
// DEFINIR TODO!!!!!!!!!!!!!!!
// DEFINIR TODO!!!!!!!!!!!!!!!
const unsigned int BTN_UP = 0;
const unsigned int BTN_DOWN = 0;
const unsigned int BCD_DEC_A = 0;
const unsigned int BCD_DEC_B = 0;
const unsigned int BCD_DEC_C = 0;
const unsigned int BCD_DEC_D = 0;
const unsigned int BCD_UNI_A = 0;
const unsigned int BCD_UNI_B = 0;
const unsigned int BCD_UNI_C = 0;
const unsigned int BCD_UNI_D = 0;
// DEFINIR TODO!!!!!!!!!!!!!!!
// DEFINIR TODO!!!!!!!!!!!!!!!
// DEFINIR TODO!!!!!!!!!!!!!!!
// DEFINIR TODO!!!!!!!!!!!!!!!

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
const int TARGET_MAX = 25;
const int TARGET_MIN = 0;

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
	
	pinMode(BCD_DEC_A, OUTPUT);
	pinMode(BCD_DEC_B, OUTPUT);
	pinMode(BCD_DEC_C, OUTPUT);
	pinMode(BCD_DEC_D, OUTPUT);
	pinMode(BCD_UNI_A, OUTPUT);
	pinMode(BCD_UNI_B, OUTPUT);
	pinMode(BCD_UNI_C, OUTPUT);
	pinMode(BCD_UNI_D, OUTPUT);
	
	digitalWrite(BCD_DEC_A, LOW);
	digitalWrite(BCD_DEC_B, LOW);
	digitalWrite(BCD_DEC_C, LOW);
	digitalWrite(BCD_DEC_D, LOW);
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
		outputIntToBcd(dec, BCD_DEC_A, BCD_DEC_B, BCD_DEC_C, BCD_DEC_D);
		outputIntToBcd(uni, BCD_UNI_A, BCD_UNI_B, BCD_UNI_C, BCD_UNI_D);
	}
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
