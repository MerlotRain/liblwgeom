/*****************************************************************************/
/*  Math Spatial Engine - Open source 2D geometry algorithm library          */
/*                                                                           */
/*  Copyright (C) 2013-2024 Merlot.Rain                                      */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 3 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*****************************************************************************/

#ifndef NMEA_H
#define NMEA_H

#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>
#include <mathse.h>

enum NMEA_sentence_id {
    NMEA_INVALID = -1,
    NMEA_UNKNOWN = 0,
    NMEA_GBS,
    NMEA_GGA,
    NMEA_GLL,
    NMEA_GSA,
    NMEA_GST,
    NMEA_GSV,
    NMEA_RMC,
    NMEA_VTG,
    NMEA_ZDA,
};

struct nmea_gpgga
{
    struct tm* utc;
    double lat;
    char ns;
    double lon;
    char ew;
    int sig;
    int satinuse;
    double HDOP;
    double elv;
    char elv_unit;
    double diff;
    char diff_units;
    double dgps_age;
    int dgps_sid;
};

#ifdef __cplusplus
}
#endif

#endif