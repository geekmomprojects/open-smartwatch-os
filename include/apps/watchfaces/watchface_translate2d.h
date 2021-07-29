#ifndef OSW_APP_WATCHFACE_TRANSLATE2D_H
#define OSW_APP_WATCHFACE_TRANSLATE2D_H

#include <osw_hal.h>
#include <osw_ui.h>

#include "osw_app.h"

class OswAppWatchfaceTranslate2D : public OswApp {
 public:
  OswAppWatchfaceTranslate2D(void) { ui = OswUI::getInstance(); };
  void setup(OswHal* hal);
  void loop(OswHal* hal);
  void stop(OswHal* hal);
  ~OswAppWatchfaceTranslate2D(){};

 private:
  OswUI* ui;
  void drawWatch(OswHal* hal);
};

#endif
