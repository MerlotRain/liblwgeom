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

#define NMEA_MAXSAT    (12)
#define NMEA_SATINPACK (4)
#define NMEA_NSATPACKS (NMEA_MAXSAT / NMEA_SATINPACK)

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

/// Information about satellite
struct nmea_SATELLITE {
    int id;      /**< Satellite PRN number */
    int in_use;  /**< Used in position fix */
    int elv;     /**< Elevation in degrees, 90 maximum */
    int azimuth; /**< Azimuth, degrees from true north, 000 to 359 */
    int sig;     /**< Signal, 00-99 dB */
};

/// GGA packet information structure (Global Positioning System Fix Data)
struct nmea_GPGGA {
    struct tm utc; /**< UTC of position (just time) */
    double lat;    /**< Latitude in NDEG - [degree][min].[sec/60] */
    char ns;       /**< [N]orth or [S]outh */
    double lon;    /**< Longitude in NDEG - [degree][min].[sec/60] */
    char ew;       /**< [E]ast or [W]est */
    int sig; /**< GPS quality indicator (0 = Invalid; 1 = Fix; 2 = Differential,
                3 = Sensitive) */
    int satinuse;   /**< Number of satellites in use (not those in view) */
    double HDOP;    /**< Horizontal dilution of precision */
    double elv;     /**< Antenna altitude above/below mean sea level (geoid) */
    char elv_units; /**< [M]eters (Antenna height unit) */
    double
        diff; /**< Geoidal separation (Diff. between WGS-84 earth ellipsoid and
                 mean sea level. '-' = geoid is below WGS-84 ellipsoid) */
    char diff_units; /**< [M]eters (Units of geoidal separation) */
    double dgps_age; /**< Time in seconds since last DGPS update */
    int dgps_sid;    /**< DGPS station ID number */
};

/// GSA packet information structure (Satellite status)
struct nmea_GPGSA {
    char fix_mode; /**< Mode (M = Manual, forced to operate in 2D or 3D; A =
                      Automatic, 3D/2D) */
    int fix_type; /**< Type, used for navigation (1 = Fix not available; 2 = 2D;
                     3 = 3D) */
    int sat_prn[NMEA_MAXSAT]; /**< PRNs of satellites used in position fix (null
                                 for unused fields) */
    double PDOP;              /**< Dilution of precision */
    double HDOP;              /**< Horizontal dilution of precision */
    double VDOP;              /**< Vertical dilution of precision */
};

/// GSV packet information structure (Satellites in view)
struct nmea_GPGSV {
    int pack_count; /**< Total number of messages of this type in this cycle */
    int pack_index; /**< Message number */
    int sat_count;  /**< Total number of satellites in view */
    nmea_SATELLITE sat_data[NMEA_SATINPACK];
};

/// RMC packet information structure (Recommended Minimum sentence C)
struct nmea_GPRMC {
    struct tm utc;      /**< UTC of position */
    char status;        /**< Status (A = active or V = void) */
    double lat;         /**< Latitude in NDEG - [degree][min].[sec/60] */
    char ns;            /**< [N]orth or [S]outh */
    double lon;         /**< Longitude in NDEG - [degree][min].[sec/60] */
    char ew;            /**< [E]ast or [W]est */
    double speed;       /**< Speed over the ground in knots */
    double direction;   /**< Track angle in degrees True */
    double declination; /**< Magnetic variation degrees (Easterly var. subtracts
                           from true course) */
    char declin_ew;     /**< [E]ast or [W]est */
    char mode;          /**< Mode indicator of fix type (A = autonomous, D =
                           differential, E = estimated, N = not valid, S = simulator) */
};

/**
 * VTG packet information structure (Track made good and ground speed)
 */
struct nmea_GPVTG {
    double dir; /**< True track made good (degrees) */
    char dir_t; /**< Fixed text 'T' indicates that track made good is relative
                   to true north */
    double dec; /**< Magnetic track made good */
    char dec_m; /**< Fixed text 'M' */
    double spn; /**< Ground speed, knots */
    char spn_n; /**< Fixed text 'N' indicates that speed over ground is in knots
                 */
    double spk; /**< Ground speed, kilometers per hour */
    char spk_k; /**< Fixed text 'K' indicates that speed over ground is in
                   kilometers/hour */

} nmea_GPVTG;

#ifdef __cplusplus
}
#endif

#endif