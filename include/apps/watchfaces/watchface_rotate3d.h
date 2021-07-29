#ifndef OSW_APP_WATCHFACE_ROTATE3D_H
#define OSW_APP_WATCHFACE_ROTATE3D_H

#include <osw_hal.h>
#include <osw_ui.h>

#include "osw_app.h"
#include "projection.h"

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
  Transform watch_trans; // transformation matrix for watchface

};

#endif
