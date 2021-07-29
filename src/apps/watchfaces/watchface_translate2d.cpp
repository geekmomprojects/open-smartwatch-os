
#include "./apps/watchfaces/watchface_translate2d.h"
// #define GIF_BG

#include <gfx_util.h>
#include <osw_app.h>
#include <osw_config.h>
#include <osw_config_keys.h>
#include <osw_hal.h>

#ifdef GIF_BG
#include "./apps/_experiments/gif_player.h"
#endif

const int middleX = 120;
const int middleY = 120;

void drawDateTranslate(OswHal* hal, uint8_t x0 = 120, uint8_t y0 = 120, const bool& useMMDDYYYY=true) {
  string day = "";
  uint32_t dayInt = 0;
  uint32_t monthInt = 0;
  uint32_t yearInt = 0;
  int charLen = 3;
  hal->getWeekdayString(charLen, &day);
  hal->getDate(&dayInt, &monthInt, &yearInt);

  // we want to output a value like "Wed, 05/02/2021"
  char date_Array[charLen + 1];

  strcpy(date_Array, day.c_str());

  hal->gfx()->setTextSize(2);
  hal->gfx()->setTextMiddleAligned();
  hal->gfx()->setTextLeftAligned();
  hal->gfx()->setTextCursor(x0 - hal->gfx()->getTextOfsetColumns(6.9), y0 - 40);

  hal->gfx()->print(date_Array);
  hal->gfx()->print(", ");

  // i really would want the date to be dynamic based on what's in the config, but the most efficient thing to do right
  // now is simply two if statements covering the 2 common conditions.
  if (useMMDDYYYY) {
    hal->gfx()->printDecimal(monthInt, 2);
    hal->gfx()->print("/");
    hal->gfx()->printDecimal(dayInt, 2);
    hal->gfx()->print("/");
  } else {
    hal->gfx()->printDecimal(dayInt, 2);
    hal->gfx()->print(".");
    hal->gfx()->printDecimal(monthInt, 2);
    hal->gfx()->print(".");
  }

  hal->gfx()->print(yearInt);
}

void OswAppWatchfaceTranslate2D::drawWatch(OswHal* hal) {


  /*
  uint32_t steps = hal->getStepCount();
  hal->gfx()->drawArc(120, 120, 0, 360 * (steps / 10800.0), 90, 93, 6,
                      steps > 10800 ? ui->getSuccessColor() : ui->getInfoColor(), true);
  */
  // below two arcs take too long to draw

  // hal->gfx()->drawArc(120, 120, 0, 360, 180, 75, 7, changeColor(COLOR_GREEN, 0.25));
  // hal->gfx()->drawArc(120, 120, 0, (steps / 360) % 360, 180, 75, 7, dimColor(COLOR_GREEN, 25));
  // hal->gfx()->drawArc(120, 120, 0, (steps / 360) % 360, 180, 75, 6, COLOR_GREEN);

  // float bat = hal->getBatteryPercent() * 3.6;

  // hal->gfx()->drawArc(120, 120, 0, 360, 180, 57, 7, changeColor(COLOR_BLUE, 0.25));
  // hal->gfx()->drawArc(120, 120, 0, bat, 180, 57, 7, dimColor(COLOR_BLUE, 25));
  // hal->gfx()->drawArc(120, 120, 0, bat, 180, 57, 6, COLOR_BLUE);

  uint32_t second = 0;
  uint32_t minute = 0;
  uint32_t hour = 0;
  unsigned char r,g,b;
  uint16_t sec_color;
  hal->getLocalTime(&hour, &minute, &second);

  
  const float xValue = hal->getAccelerationX();
  const float yValue = hal->getAccelerationY();
  //const float zValue = hal->getAccelerationZ();
  const int x0 = middleX + xValue * 64;
  const int y0 = middleY - yValue * 64;

  const bool isXYAccelerationInMiddle = abs(yValue) < 0.25 && abs(xValue) < 0.25;

  uint16_t color = isXYAccelerationInMiddle ? ui->getInfoColor()  : ui->getDangerColor() ;
 

  hal->gfx()->drawMinuteTicks(x0, y0, 116, 112, ui->getForegroundDimmedColor());
  //hal->gfx()->drawHourTicks(x0, y0, 117, 107, ui->getForegroundColor());

  // get color associated with seconds
  hsvToRgb((uint8_t)255*second/60,255,255,r,g,b);
  sec_color = rgb565(r,g,b);

  // hours
  hal->gfx()->drawThickTick(x0, y0, 0, 16, 360.0 / 12.0 * (1.0 * hour + minute / 60.0), 1, ui->getForegroundColor());
  hal->gfx()->drawThickTick(x0, y0, 16, 60, 360.0 / 12.0 * (1.0 * hour + minute / 60.0), 4, color);

  // minutes
  hal->gfx()->drawThickTick(x0, y0, 0, 16, 360.0 / 60.0 * (1.0 * minute + second / 60.0), 1,
                            ui->getForegroundColor());
  hal->gfx()->drawThickTick(x0, y0, 16, 105, 360.0 / 60.0 * (1.0 * minute + second / 60.0), 4,
                            color);



  
  drawDateTranslate(hal, x0, y0);


#ifndef GIF_BG
  // seconds
  
  hal->gfx()->fillCircle(x0, y0, 3, ui->getDangerColor());
  //hal->gfx()->fillCircle((int)(120 + 90*cos(6.28*second/60)), (int)(120 + 90*sin(6.28*second/60)),second % 2 ? 6 : 8,sec_color);
  hal->gfx()->drawThickTick(x0, y0, 0, 16, 360.0 / 60.0 * second, 1, ui->getForegroundColor());
  hal->gfx()->drawThickTick(x0, y0, 0, 110, 360.0 / 60.0 * second, 1, color);
#endif

}

#ifdef GIF_BG
OswAppGifPlayer* bgGif = new OswAppGifPlayer();
#endif

void OswAppWatchfaceTranslate2D::setup(OswHal* hal) {
#ifdef GIF_BG
  bgGif->setup(hal);
#endif
}

void OswAppWatchfaceTranslate2D::loop(OswHal* hal) {
  if (hal->btnHasGoneDown(BUTTON_3)) {
    hal->increaseBrightness(25);
  }
  if (hal->btnHasGoneDown(BUTTON_2)) {
    hal->decreaseBrightness(25);
  }

#ifdef GIF_BG
  // if (millis() - 1000 > lastDraw) {
  bgGif->loop(hal);
  // lastDraw = millis();
  // }
#else
  hal->gfx()->fill(ui->getBackgroundColor());
#endif

  drawWatch(hal);
  hal->requestFlush();
}

void OswAppWatchfaceTranslate2D::stop(OswHal* hal) {
#ifdef GIF_BG
  bgGif->stop(hal);
#endif
}
