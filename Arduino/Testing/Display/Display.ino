// I/O Pins
const unsigned int BTN_UP = A1;
const unsigned int BTN_DOWN = A0;
const unsigned int BCD_TEN_A = 2;
const unsigned int BCD_TEN_B = 3;
const unsigned int BCD_TEN_C = 4;
const unsigned int BCD_TEN_D = 7;
const unsigned int BCD_UNI_A = 13;
const unsigned int BCD_UNI_B = 12;
const unsigned int BCD_UNI_C = 10;
const unsigned int BCD_UNI_D = 9;

int temp = 0;

void setup(void)
{
  Serial.begin(9600);
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

void loop(void)
{
  //Serial.println(temp);
  
	setDisplay(temp);
	temp++;
	if(temp > 40)
	{
		temp = -20;
	}
 
	delay(1000);
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
  Serial.println(ten);
  Serial.println(uni);
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
