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

bool bUp;
bool bDown;

int iTemp = 14;

void setup()
{
  Serial.begin(9600);
  setupIO();
  
  bUp = false;
  bDown = false;
}

void setupIO()
{
  pinMode(BTN_UP, INPUT);
  pinMode(BTN_DOWN, INPUT);
  
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
  readInput();
  if(bUp)
  {
    Serial.println("up");
    iTemp++;
    setDisplay(iTemp);
  }
  if(bDown)
  {
    Serial.println("down");
    iTemp--;
    setDisplay(iTemp);
  }
  delay(500);
  
  bUp = false;
  bDown = false;
  Serial.println(iTemp);
}

void readInput()
{
  bUp = HIGH == digitalRead(BTN_UP);
  bDown = HIGH == digitalRead(BTN_DOWN);
}

/**************************************************
** Function tested and working fine on 01/01/17
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

/*****************************************************
** Tested along with setDisplay, re-test if changed
*****************************************************/
void outputIntToBcd(int digit, int pinA, int pinB, int pinC, int pinD)
{
  digitalWrite(pinA, 1 == bitRead(digit, 0) ? HIGH : LOW);
  digitalWrite(pinB, 1 == bitRead(digit, 1) ? HIGH : LOW);
  digitalWrite(pinC, 1 == bitRead(digit, 2) ? HIGH : LOW);
  digitalWrite(pinD, 1 == bitRead(digit, 3) ? HIGH : LOW);
}
