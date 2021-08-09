#ifndef OSW_APP_WATCHFACE_ROTATE3D_H
#define OSW_APP_WATCHFACE_ROTATE3D_H

#include <osw_hal.h>
#include <osw_ui.h>

#include "osw_app.h"
#include "../../../lib/projection/Projection.h"
#define NUM_SEVEN_SEGMENT_NUMBER_LINES 20

class OswAppWatchfaceRotate3D : public OswApp {
 public:
  OswAppWatchfaceRotate3D(void);
  void setup(OswHal* hal);
  void loop(OswHal* hal);
  void stop(OswHal* hal);
  ~OswAppWatchfaceRotate3D(){};

 private:
  OswUI* ui;
  Camera cam;
  void drawWatch(OswHal* hal);
  float angle_roll;   //Rotation angles of watchface in radians
  float angle_pitch;
  float camera_dist;  //Distance of camera from screen
  point3 tick_start[60]; // unrotated start min/hour ticks [xi,yi,zi]
  point3 tick_end[60]; // unrotated end min/hour ticks [xf,yf,zf]

  uint8_t numeral_lines[NUM_SEVEN_SEGMENT_NUMBER_LINES][2][2] = {{{111,240},{111,221}},{{117,240},{129,240}},{{129,240},{129,231}},{{129,231},{117,231}},{{117,231},{117,222}},{{117,222},{129,222}}, //12
                                                                {{228,129},{240,129}},{{228,120},{240,120}},{{228,111},{240,111}},{{240,129},{240,111}}, //3
                                                                {{126,18},{114,18}},{{114,18},{114,0}},{{114,0},{126,0}},{{126,0},{126,9}},{{126,9},{114,9}}, //6
                                                                {{0,111},{12,111}},{{12,111},{12,129}},{{12,129},{0,129}},{{0,129},{0,120}},{{0,120},{12,120}}}; //9
                  
  Transform watch_trans; // transformation matrix for watchface

};

#endif
