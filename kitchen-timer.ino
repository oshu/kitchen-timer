/* Timer using the DFRobot LED Keypad Shield */

#include <LedKeypad.h>
#include <TimeLib.h>

const int brightness = 0;

const int buzzer = 9; //buzzer to arduino pin 9

const int defaultHours = 0; // the timer does not have an 'hours' display
const int defaultMinutes = 7;
const int defaultSeconds = 0;

const int finalSeconds = 5; // these get a beep

int timerRunning = false;

int timerHours = 0;
int timerMinutes = 0;
int timerSeconds = 0;

time_t setupTime = 0;
time_t startTime = 0;
time_t currentTime = 0;


void updateDisplay() {
    ledkeypad.display(0,(timerMinutes / 10));
    ledkeypad.display(1,(timerMinutes % 10));
    ledkeypad.display(2,(timerSeconds / 10));
    ledkeypad.display(3,(timerSeconds % 10));
}


void updateTimer() {
    
    if(timerRunning == true) {
        
        if(timerMinutes > 0 || timerSeconds > 0) {
            currentTime = setupTime - (now() - startTime);
            timerMinutes = minute(currentTime);
            
            if(timerSeconds != second(currentTime)) {
                timerSeconds = second(currentTime);
                updateDisplay();
                
                if(timerMinutes == 0 && timerSeconds <= finalSeconds) {
                    // final countdown
                    playBeep();
                }
            }
            
        } else {
            // time is up
            timerRunning = false;
            playAlarm();
        }
    }
}


void playAlarm() {
    
    // boop boop
    for (int i = 0; i <= 3; i++) {
        tone(buzzer, 1000);
        ledkeypad.setBrightness(brightness);
        delay(300);
        tone(buzzer, 50);
        ledkeypad.setBrightness(7);
        delay(400);
    }
    
    noTone(buzzer);
    ledkeypad.setBrightness(brightness);
}


void playBeep() {
    tone(buzzer, 2000);
    delay(200);
    noTone(buzzer);
}


void setup() {
    // set up the buzzer
    pinMode(buzzer, OUTPUT);

    // setup the display
    ledkeypad.begin();
    ledkeypad.setBrightness(brightness);
    ledkeypad.dotShow(1);
    
    setupTime = defaultSeconds + (60 * defaultMinutes) + (3600 * defaultHours);
    
    // set the timer to the default time
    currentTime = setupTime;
    timerHours = defaultHours;
    timerMinutes = defaultMinutes;
    timerSeconds = defaultSeconds;
    
    updateDisplay();
}


void loop() {
    unsigned char keyValue=0;
    
    updateTimer();

    // read the keypad
    keyValue = ledkeypad.getKey();
    switch(keyValue){

        case KEY_LEFT: // add 1 minute
            if(timerRunning == false) {
                timerMinutes++;
            }
            
            updateDisplay();
            break;


        case KEY_RIGHT: // add 10 seconds
            if(timerRunning == false) {
                if(timerSeconds >= 50) {
                    timerMinutes++;
                    timerSeconds = timerSeconds - 50;
                } else {
                    timerSeconds = timerSeconds + 10;
                }
            }
            
            updateDisplay();
            break;


        case KEY_DOWN: // zero out the timer
            if(timerRunning == false) {
                timerMinutes = 0;
                timerSeconds = 0;
            }
            
            updateDisplay();
            break;


        case KEY_UP: // set default time
            if(timerRunning == false) {
                timerMinutes = defaultMinutes;
                timerSeconds = defaultSeconds;
            }
            
            updateDisplay();
            break;


        case KEY_SELECT: // start or stop the timer
            if(timerRunning == true) {
                timerRunning = false;
            } else {
                startTime = now();
                setupTime = timerSeconds + (60 * timerMinutes) + (3600 * timerHours);
                timerRunning = true;
            }
            
            updateDisplay();
            break;


        default:
            break;
    }
}
