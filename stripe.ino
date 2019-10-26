//stripe.ino
#include <OneButton.h>
#include <NeoPixelBus.h>

#define DEFAULTBRIGHT 200 	//Standard lysnivå
#define TIMEOUT 120000 		//2min + ca 17 på pir timeout,

#define PWMRED 11
#define PWMGREEN 10
#define PWMBLUE 9
#define PWMWHITE 5 

const uint16_t PixelCount = 100; // this example assumes 4 pixels, making it smaller will cause a failure
const uint8_t PixelPin = 6;  // make sure to set this to the correct pin, ignored for Esp8266
bool disco = false;
bool strobe = false;
bool doTimeout = true; 

unsigned long timeout;

NeoPixelBus<NeoRgbwFeature, NeoSk6812Method> strip(PixelCount, PixelPin);

OneButton buttonRed(A0,true);
OneButton buttonGreen(A1,true);
OneButton buttonBlue(A2,true);
OneButton buttonWhite(A3,true);

RgbwColor black(0);
RgbwColor current(0);
RgbwColor white(200,200,200,200);

struct MyAnimationState
{
    RgbwColor StartingColor;  // the color the animation starts at
    RgbwColor EndingColor; // the color the animation will end at};
};

MyAnimationState animationState[PixelCount];

void SetRandomSeed()
{
    uint32_t seed;

    // random works best with a seed that can use 31 bits
    // analogRead on a unconnected pin tends toward less than four bits
    seed = analogRead(A7);
    delay(1);

    for (int shifts = 3; shifts < 31; shifts += 3)
    {
        seed ^= analogRead(A7) << shifts;
        delay(1);
    }

    // Serial.println(seed);
    randomSeed(seed);
}

void SetupAnimationSet()
{
    // setup some animations
    for (uint16_t pixel = 0; pixel < PixelCount; pixel++)
    {
        const uint8_t peak = 128;

        // each animation starts with the color that was present
        RgbwColor originalColor = strip.GetPixelColor(pixel);
        // and ends with a random color
        RgbwColor targetColor = RgbwColor(random(peak), random(peak), random(peak),0);

        // each animation starts with the color that was present
        animationState[pixel].StartingColor = originalColor;
        // and ends with a random color
        animationState[pixel].EndingColor = targetColor;
    }
}


void setup() {
  	Serial.begin(115200);
    while (!Serial); // wait for serial attach
    Serial.println();
    Serial.println("Initializing...");
    Serial.flush();

    // this resets all the neopixels to an off state
    strip.Begin();
    strip.Show();

    Serial.println();
    Serial.println("Running...");

    buttonRed.attachClick(clickRed);
    buttonGreen.attachClick(clickGreen);
    buttonBlue.attachClick(clickBlue);
    buttonWhite.attachClick(clickWhite);

    buttonRed.attachDoubleClick(clickRedOff);
    buttonGreen.attachDoubleClick(clickGreenOff);
    buttonBlue.attachDoubleClick(clickBlueOff);
    buttonWhite.attachDoubleClick(clickWhiteOff);

    buttonRed.attachLongPressStop(longPressRed);
    buttonGreen.attachLongPressStop(longPressGreen);
    buttonBlue.attachLongPressStop(longPressBlue);
    buttonWhite.attachLongPressStop(longPressWhite);

    SetRandomSeed();

    pinMode(A4, INPUT);
}

void loop() {
	// put your main code here, to run repeatedly:
	buttonRed.tick();
	buttonGreen.tick();
	buttonBlue.tick();
	buttonWhite.tick();
	delay(10);
	if (disco) doDisco();
	if (strobe) doStrobe();

	if(!doTimeout) return;

	if (digitalRead(A4)) {
		timeout=millis();
		Serial.println(current.R);
		if (current.R!=DEFAULTBRIGHT) {

			Serial.println("turning on light :)");
			setBrighter(current.R);
			doTimeout=true;
		}
	}
	else if( millis()-timeout > TIMEOUT ) {
		setAllOff();
	}
}

void clickRed() {setBrighter(current.R);}
void clickGreen() {setBrighter(current.G);}
void clickBlue() {setBrighter(current.B);}
void clickWhite() {setBrighter(current.W);}

void clickRedOff() {setOff(current.R);}
void clickGreenOff() {setOff(current.G);}
void clickBlueOff() {setOff(current.B);}
void clickWhiteOff() {setAllOff();}

void longPressRed() {
    doTimeout=false;
	disco=true;
}
void longPressGreen() {
}
void longPressBlue() {
}
void longPressWhite() {
    doTimeout=false;
	strobe=true;
}

void setOff(uint8_t &color) {
	doTimeout=false;
	color=0;
	for(uint16_t i=0; i<PixelCount; i++) {
    	strip.SetPixelColor(i, current);
    }
    strip.Show();

    analogWrite(PWMRED,current.R);
    analogWrite(PWMGREEN,current.G);
    analogWrite(PWMBLUE,current.B);
    analogWrite(PWMWHITE,current.W);
}

void setAllOff() {
	current=black;
	for(uint16_t i=0; i<PixelCount; i++) {
    	strip.SetPixelColor(i, black);
    }
    strip.Show();
    analogWrite(PWMRED,current.R);
    analogWrite(PWMGREEN,current.G);
    analogWrite(PWMBLUE,current.B);
    analogWrite(PWMWHITE,current.W);
    doTimeout=true;	
}

void setBrighter(uint8_t &color) {
    strobe=false;
    disco=false;
    doTimeout=false;

	if(color==0) color=DEFAULTBRIGHT;
	else color+=30;

	for(uint16_t i=0; i<PixelCount; i++) {
    	strip.SetPixelColor(i, current);
    }
    strip.Show();

    analogWrite(PWMRED,current.R);
    analogWrite(PWMGREEN,current.G);
    analogWrite(PWMBLUE,current.B);
    analogWrite(PWMWHITE,current.W);
}

void doDisco() {
	SetupAnimationSet();
	for(float t=0; t<1.f; t+=0.1f) {
		for(uint16_t i=0; i<PixelCount; i++) {
			strip.SetPixelColor(i, RgbwColor::LinearBlend(
				animationState[i].StartingColor,
				animationState[i].EndingColor,
				t
				));
		}
		strip.Show();
	}
	delayButtons(100);
}

void doStrobe() {
		for(uint16_t i=0; i<PixelCount; i++) {
    		strip.SetPixelColor(i, white);
    	}
    	strip.Show();
    	delayButtons(200);

    	for(uint16_t i=0; i<PixelCount; i++) {
    		strip.SetPixelColor(i, black);
    	}
    	strip.Show();
    	delayButtons(200);	
}

void delayButtons(unsigned long ms) {
	unsigned long t = millis(); 
    	while((millis()-t) < ms) {
    		buttonRed.tick();
    		buttonGreen.tick();
    		buttonBlue.tick();
    		buttonWhite.tick();
    		delay(10);
    	}
}
