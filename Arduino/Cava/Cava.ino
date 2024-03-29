// I/O Pins
const unsigned int BTN_UP = 15;
const unsigned int BTN_DOWN = 14;
const unsigned int BCD_TEN_A = 2;
const unsigned int BCD_TEN_B = 3;
const unsigned int BCD_TEN_C = 4;
const unsigned int BCD_TEN_D = 7;
const unsigned int BCD_UNI_A = 13;
const unsigned int BCD_UNI_B = 12;
const unsigned int BCD_UNI_C = 10;
const unsigned int BCD_UNI_D = 9;

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
	Serial.begin(9600);
	
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
	ulLastTime = micros();
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
	//Serial.print("State: ");
	//Serial.println(eInputState);
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

/**************************************************
** Function tested and workking fine on 01/01/17
** re-test if anything is changed
** find test file at Cava\Arduino\Testing\Buttons
**************************************************/
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
		Serial.println("GO TO CHANGE");
		if(bDisplayCurrent)
		{
			bDisplayCurrent = false;
		}
		else
		{
			if(bUp)
			{
				Serial.println("+1 FROM IDLE");
				iNextTarget = min(iNextTarget + 1, TARGET_MAX);
			}
			else
			{
				Serial.println("-1 FROM IDLE");
				iNextTarget = max(iNextTarget - 1, TARGET_MIN);
			}
		}
		bUpdateDisplay = true;
	}
	else
	{
		if(!bDisplayCurrent)
		{
			fInputTimer += dt;
			//Serial.print("fInputTimer (to show current): ");
			//Serial.println(fInputTimer);
			if(fInputTimer >= TARGET_DELAY)
			{
				Serial.println("SHOW CURRENT");
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
		//Serial.print("fInputTimer (to keep change): ");
		//Serial.println(fInputTimer);
		if(fInputTimer >= HOLD_DELAY)
		{
			if(bUp)
			{
				Serial.println("+1 DUE TO HOLD");
				iNextTarget = min(iNextTarget + 1, TARGET_MAX);
			}
			else
			{
				Serial.println("-1 DUE TO HOLD");
				iNextTarget = max(iNextTarget - 1, TARGET_MIN);
			}
			bUpdateDisplay = true;
			fInputTimer = 0;
		}
	}
	else
	{
		Serial.println("GO TO IDLE");
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
		bUpdateDisplay = false;
		Serial.println("Updating display");
		if(bDisplayCurrent)
		{
			setDisplay(iTemp);
		}
		else
		{
			setDisplay(iNextTarget);
		}
	}
}

/**************************************************
** Function tested and workking fine on 01/01/17
** re-test if anything is changed
** find test file at Cava\Arduino\Testing\Display
**************************************************/
void setDisplay(int num)
{
	int ten = 0;
	int uni = 0;

	// TODO: what do we do when current temperature is outside this ranges?
	// user plugs device at -5º -> display is unreadable -> user returns 
	// malfunctioning device
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

/*
** Tested along with setDisplay, re-test if changed
*/
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
	unsigned long ulNewTime = micros();
	unsigned long ul_dt = 0;
	if(ulNewTime > ulLastTime)
	{
		ul_dt = ulNewTime - ulLastTime;
	}
	else
	{
		ul_dt = MAX_UL - ulLastTime + ulNewTime;
	}
	float f_dt_sec = (float)ul_dt / 1000000.0f;
	//Serial.print("Delta time: ");
	//Serial.println(f_dt_sec);
	ulLastTime = ulNewTime;
	return f_dt_sec;
}
