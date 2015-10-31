#ifndef PCH_H
#define PCH_H

#include "windows.h"

//standard borland headers --------------------------
#include <vcl.h>

#include <Data.DBXJSON.hpp>
#include <System.hpp>
#include <Vcl.Grids.hpp>
#include <IdHTTP.hpp>
#include <System.Classes.hpp>
#include <jpeg.hpp>
#include <System.DateUtils.hpp>

#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <System.Math.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.AppEvnts.hpp>
/*
//commend because one of the header file initialize the data in the header
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>

*/
/*
#include <System.SysUtils.hpp>
//#include <Xml.XMLDoc.hpp>
//#include <Xml.XMLIntf.hpp>
*/

//standard headers ----------------------------------
#include <assert.h>
#include <tchar.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <float.h>
#include <math.h>
//#include <gl/gl.h>
#include <TlHelp32.h>
#include <cstdio>
#include <float.h>
//#include <WinDef.h>

//standard STL headers ------------------------------
#include <string>
#include <vector>
#include <map>
#include <deque>
#include <list>
#include <set>
#include <exception>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <valarray>


//global exceptions ---------------------------------
#include "exceptions.h"

//external sys headers ------------------------------
#include "crc.h"
#include "sysSystem.h"
#include "sysBaseTypes.h"
#include "sysException.h"
#include "sysFile.h"
#include "sysStr.h"
#include "sysJSON.h"
#include "sysTime.h"
#include "sysKey.h"
#include "sysMath.h"
#include "sysStr.h"
#include "sysObserver.h"
#include "sysObserverable.h"
#include "sysPatterns.h"
#include "sysProcess.h"
#include "sysReg.h"
#include "sysThread.h"
#include "sysLogger.h"
#include "sysControlState.h"
#include "sysApp.h"
#include "sysOO.h"
#include "sysVCL.h"

//datasource object
#include "datasource_object.h"

//datasource
#include "datasource.h"
#include "datasource_vector.h"
#include "datasource_list.h"
#include "datasource_string_grid.h"

//string grid
#include "string_grid_datasource.h"

//external types
#include "briz_analog_mode.h"
#include "data_type.h"
#include "data_file_formats.h"
#include "event_type.h"
#include "alarm_data_type.h"
#include "unit.h"
#include "power.h"
#include "sensor_data.h"
#include "sensor_event_data.h"
#include "chart_mouse_strategy.h"
#include "observerable_types.h"
#include "trackable_type.h"

//external records

#include "record.h"
#include "user.h"
#include "collection_point.h"
#include "device.h"
#include "sensor.h"
#include "sensor_bit.h"
#include "device_tree_group.h"
#include "records/mnemoshema.h"
#include "img.h"

//global variables
#include "global.h"

//borland library
#include "db_module.h"

#endif
