#include "settings.h"

#include "hw_config.h"


#include "settings_eeprom.h"

#include "channels.h"
#include "receiver.h"
#include "receiver_spi.h"

#include "buttons.h"
#include "state.h"
#include "ui.h"



static void globalMenuButtonHandler(
    Button button,
    Buttons::PressType pressType
);

void setup() {
  // init pins
  setupPins();
  StateMachine::setup();
  Ui::setup();

  Receiver::setActiveReceiver(Receiver::ReceiverId::B);
  Buttons::registerChangeFunc(globalMenuButtonHandler);
 // Switch to initial state.
  StateMachine::switchState(StateMachine::State::SEARCH);
  
}

void setupPins() {

    pinMode(PIN_BUZZER, OUTPUT);
    pinMode(PIN_BUTTON_UP, INPUT_PULLUP);
    pinMode(PIN_BUTTON_MODE, INPUT_PULLUP);
    pinMode(PIN_BUTTON_DOWN, INPUT_PULLUP);
    pinMode(PIN_BUTTON_EB0, INPUT_PULLUP);
    pinMode(PIN_BUTTON_EB1, INPUT_PULLUP);
    pinMode(PIN_BUTTON_EB2, INPUT_PULLUP);

    pinMode(PIN_LED_A,OUTPUT);
    #ifdef USE_DIVERSITY
        pinMode(PIN_LED_B,OUTPUT);
    #endif

    pinMode(PIN_RSSI_A, INPUT_ANALOG);
    #ifdef USE_DIVERSITY
        pinMode(PIN_RSSI_B, INPUT_ANALOG);
    #endif

    pinMode(PIN_SW, OUTPUT);

    pinMode(PIN_SPI_DATA, OUTPUT);
    pinMode(PIN_SPI_SLAVE_SELECT_A, OUTPUT);
    pinMode(PIN_SPI_SLAVE_SELECT_B, OUTPUT);
    
    pinMode(PIN_SPI_CLOCK, OUTPUT);
    digitalWrite(PIN_SPI_SLAVE_SELECT_A, HIGH);
    digitalWrite(PIN_SPI_SLAVE_SELECT_B, HIGH);
    digitalWrite(PIN_SPI_CLOCK, LOW);
    digitalWrite(PIN_SPI_DATA, LOW);
}



void loop() {
  // put your main code here, to run repeatedly:
    Receiver::update();
    Buttons::update();
    StateMachine::update();
    Ui::update();
    EepromSettings.update();

    if (
        StateMachine::currentState != StateMachine::State::SCREENSAVER
        && StateMachine::currentState != StateMachine::State::BANDSCAN
        && (millis() - Buttons::lastChangeTime) >
            (SCREENSAVER_TIMEOUT * 1000)
    ) {
        StateMachine::switchState(StateMachine::State::SCREENSAVER);
    }
}

static void globalMenuButtonHandler(
    Button button,
    Buttons::PressType pressType
) {
    if (
        StateMachine::currentState != StateMachine::State::MENU &&
        button == Button::MODE &&
        pressType == Buttons::PressType::HOLDING
    ) {
        StateMachine::switchState(StateMachine::State::MENU);
    }
    
}

