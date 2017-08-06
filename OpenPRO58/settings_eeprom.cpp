#include <string.h>
#include "extEEPROM.h"

#include "settings.h"
#include "settings_internal.h"
#include "settings_eeprom.h"

#include "timer.h"


static Timer saveTimer = Timer(EEPROM_SAVE_TIME);
static bool isDirty = false;

struct EepromSettings EepromSettings;

extEEPROM eep(kbits_2, 1, 8);

void EepromSettings::update() {
   // eep.begin(eep.twiClock400kHz);
    if (isDirty) {
        if (saveTimer.hasTicked()) {
            isDirty = false;
            saveTimer.reset();

            this->save();
        }
    }
}

void EepromSettings::load() {
 //   EEPROM.get(0, *this);
 //eep.read(0,&EepromSettings, sizeof(EepromSettings));

    if (this->magic != EEPROM_MAGIC)
        this->initDefaults();
}

void EepromSettings::save() {
  //  EEPROM.put(0, *this);
}

void EepromSettings::markDirty() {
    isDirty = true;
}


void EepromSettings::initDefaults() {
    memcpy_P(this, &EepromDefaults, sizeof(EepromDefaults));
    this->save();
}
