
#include "./apps/watchfaces/watchface_rotate3d.h"
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
const int zplane = 15;

// a function to return the position of the points which constitute the line segments 
void getNumberLines() {}


OswAppWatchfaceRotate3D::OswAppWatchfaceRotate3D() : cam(Camera(240,240)) { 
  ui = OswUI::getInstance(); 
  float tickStartRadius, angle;

  // Move the camera back a bit along the Z axis
  cam.transform.z = -100;
  cam.transform.x = middleX;
  cam.transform.y = middleY;

  // Initialize tick positions
  for (int i = 0; i < 60; i++) {
    angle = DEG_TO_RAD_MACRO(i*6);
    tick_end[i].x = middleX + 116*cos(angle);
    tick_end[i].y = middleY + 116*sin(angle);
    tick_end[i].z = zplane;
    if (i % 5) {
      tickStartRadius = 108;
    } else {
      tickStartRadius = 102;
    }
    tick_start[i].x = middleX + tickStartRadius*cos(angle);
    tick_start[i].y = middleY + tickStartRadius*sin(angle);
    tick_start[i].z = zplane;
  }

  // Initialize numbering positions and line segments


}

void drawDateTest(OswHal* hal, uint8_t x0 = 120, uint8_t y0 = 120, const bool& useMMDDYYYY=true) {
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

void OswAppWatchfaceRotate3D::drawWatch(OswHal* hal) {


  uint32_t second = 0;
  uint32_t minute = 0;
  uint32_t hour = 0;
  unsigned char r,g,b;
  uint16_t sec_color = ui->getDangerColor();
  uint16_t min_color = ui->getSuccessColor();
  uint16_t hour_color = ui->getInfoColor();
  uint16_t red_color = ui->getSuccessColor();
  uint16_t white_color = ui->getForegroundColor();
  hal->getLocalTime(&hour, &minute, &second);
  hour = hour % 12; // eliminate 24-hour time for angle calculation


  //const float xValue = hal->getAccelerationX();
  //const float yValue = hal->getAccelerationY();

  //const float xAngle = xValue*64;
  //const float yAngle = yValue*64;



  const float xAngle = 36*(millis() % 13000)/1300.0;
  const float yAngle = 36*(millis() % 9000)/900.0;
  const float zTrans = 40;

  //const float zValue = hal->getAccelerationZ();
  const int x0 = middleX;
  const int y0 = middleY;


  
  //Translate to origin, rotate, translate back
  Transform trans0(0,0,0,1,1,1,-x0,-y0,0);
  Transform trans1(yAngle, xAngle, 0, 1, 1, 1, 0, 0, zTrans);
  Transform trans2(0,0,0,1,1,1,x0,y0,0);
  Transform trans = trans2 * trans1 * trans0;
  
  point3 ticks, ticke;

  for (int i = 0; i < 60; i ++) {
    if (i % 15 == 0) { //Don't draw lines where numerals will be
      continue;
    }
    ticks = trans*tick_start[i];
    ticke = trans*tick_end[i];
    line3 tline = {ticks, ticke};
 
    line2 sline = cam.project(tline, false, false);
    if (!isnan(sline.p0.x) && !isnan(sline.p1.x)) {
      hal->gfx()->drawLine((int)sline.p0.x, (int)sline.p0.y, (int)sline.p1.x, (int)sline.p1.y, i % 5 ? red_color : white_color);
    }
    
  }

  for (int i = 0; i < NUM_SEVEN_SEGMENT_NUMBER_LINES; i++) {
    ticks.x = numeral_lines[i][0][0];
    ticks.y = numeral_lines[i][0][1];
    ticks.z = zplane;
    ticke.x = numeral_lines[i][1][0];
    ticke.y = numeral_lines[i][1][1];
    ticke.z = zplane;
    ticks = trans*ticks;
    ticke = trans*ticke;
    line3 tline = {ticks, ticke};
    line2 sline = cam.project(tline, false, false);
    if (!isnan(sline.p0.x) && !isnan(sline.p1.x)) {
      hal->gfx()->drawThickLine((int)sline.p0.x, (int)sline.p0.y, (int)sline.p1.x, (int)sline.p1.y,2, sec_color);
    }
  }
  
  //const bool isXYAccelerationInMiddle = abs(yValue) < 0.25 && abs(xValue) < 0.25;
 

  //hal->gfx()->drawMinuteTicks(x0, y0, 116, 112, ui->getForegroundDimmedColor());
  //hal->gfx()->drawHourTicks(x0, y0, 117, 107, ui->getForegroundColor());

  // get color associated with seconds
  hsvToRgb((uint8_t)255*second/60,255,255,r,g,b);
  sec_color = rgb565(r,g,b);
  
  // Determine points associated with the current time
  point3 hourStart, hourEnd, minStart, minEnd, secStart, secEnd;
  float hourAngle = DEG_TO_RAD_MACRO((360.0 / 12.0 )* ((3.0-hour) - minute / 60.0));
  float minAngle = DEG_TO_RAD_MACRO((360.0 / 60.0 )* ((15.0-minute) - second / 60.0));
  float secAngle = DEG_TO_RAD_MACRO(6.0*(15.0-second));
  const float ch = cos(hourAngle);
  const float sh = sin(hourAngle);
  const float cm = cos(minAngle);
  const float sm = sin(minAngle);
  const float cs = cos(secAngle);
  const float ss = sin(secAngle);
  hourStart.x = x0 + 12*ch;
  hourStart.y = y0 + 12*cs;
  hourStart.z = zplane;
  hourEnd.x = x0 + 60*ch;
  hourEnd.y = y0 + 60*sh;
  hourEnd.z = zplane;
  minStart.x = x0 + 12*cm;
  minStart.y = y0 + 12*sm;
  minStart.z = zplane;
  minEnd.x = x0 + 100*cm;
  minEnd.y = y0 + 100*sm;
  minEnd.z = zplane;
  secStart.x = x0 + 6*cs;
  secStart.y = y0 + 6*ss;
  secStart.z = zplane;
  secEnd.x = x0 + 104*cs;
  secEnd.y = y0 + 104*ss;
  secEnd.z = zplane;
  line3 hourl = {hourStart, hourEnd};
  line3 minl = {minStart, minEnd};
  line3 secl = {secStart, secEnd};
  point3 origin = {x0,y0,zplane};
  line2 hourp = cam.project(trans*hourl);
  line2 minp = cam.project(trans*minl);
  line2 secp = cam.project(trans*secl);
  point2 originp = cam.project(trans*origin);

  hal->gfx()->drawThickLine((int)hourp.p0.x,(int)hourp.p0.y,(int)hourp.p1.x,(int)hourp.p1.y,4,hour_color);
  hal->gfx()->drawThickLine((int)minp.p0.x,(int)minp.p0.y,(int)minp.p1.x,(int)minp.p1.y,4,min_color);
  hal->gfx()->drawThickLine((int)secp.p0.x,(int)secp.p0.y,(int)secp.p1.x,(int)secp.p1.y,2,sec_color);
  hal->gfx()->fillCircle(originp.x, originp.y,3, ui->getDangerColor());



  /*
  // hours
  hal->gfx()->drawThickTick(x0, y0, 0, 16, 360.0 / 12.0 * (1.0 * hour + minute / 60.0), 1, color);
  hal->gfx()->drawThickTick(x0, y0, 16, 60, 360.0 / 12.0 * (1.0 * hour + minute / 60.0), 4, color);

  // minutes
  hal->gfx()->drawThickTick(x0, y0, 0, 16, 360.0 / 60.0 * (1.0 * minute + second / 60.0), 1,
                            color);
  hal->gfx()->drawThickTick(x0, y0, 16, 105, 360.0 / 60.0 * (1.0 * minute + second / 60.0), 4,
                            color);
 */


  
  //drawDateTest(hal, x0, y0);


#ifndef GIF_BG
   /*
  // seconds
  
  hal->gfx()->fillCircle(x0, y0, 3, ui->getDangerColor());
  //hal->gfx()->fillCircle((int)(120 + 90*cos(6.28*second/60)), (int)(120 + 90*sin(6.28*second/60)),second % 2 ? 6 : 8,sec_color);
  hal->gfx()->drawThickTick(x0, y0, 0, 16, 360.0 / 60.0 * second, 1, color);
  hal->gfx()->drawThickTick(x0, y0, 0, 110, 360.0 / 60.0 * second, 1, color);
  */
#endif

}

#ifdef GIF_BG
OswAppGifPlayer* bgGif = new OswAppGifPlayer();
#endif

void OswAppWatchfaceRotate3D::setup(OswHal* hal) {
#ifdef GIF_BG
  bgGif->setup(hal);
#endif
}

void OswAppWatchfaceRotate3D::loop(OswHal* hal) {
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

void OswAppWatchfaceRotate3D::stop(OswHal* hal) {
#ifdef GIF_BG
  bgGif->stop(hal);
#endif
}
