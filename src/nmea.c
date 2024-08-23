/**
 * Copyright (c) 2023-present Merlot.Rain
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

// #include "nv.h"
// #include <stdarg.h>
// #include <stdlib.h>
// #include <stdio.h>
// #include <ctype.h>
// #include <string.h>
// #include <limits.h>
// #include <locale.h>
// #include <math.h>

// #define LOG_ERROR(...)

// #if defined(_MSC_VER)
// #define NMEA_POSIX(x) _##x
// #define NMEA_INLINE   __inline
// #else
// #define NMEA_POSIX(x) x
// #define NMEA_INLINE   inline
// #endif

// #define NMEA_TOKS_COMPARE   (1)
// #define NMEA_TOKS_PERCENT   (2)
// #define NMEA_TOKS_WIDTH     (3)
// #define NMEA_TOKS_TYPE      (4)

// #define NMEA_CONVSTR_BUF    (256)
// #define NMEA_TIMEPARSE_BUF  (256)

// #define NMEA_FIX_BAD        (1)
// #define NMEA_FIX_2D         (2)
// #define NMEA_FIX_3D         (3)

// #define NMEA_SIG_BAD        (0)
// #define NMEA_SIG_LOW        (1)
// #define NMEA_SIG_MID        (2)
// #define NMEA_SIG_HIGH       (3)

// #define NMEA_TUD_YARDS      (1.0936) //!< Yeards, meter * NMEA_TUD_YARDS = yard
// #define NMEA_TUD_KNOTS      (1.852) //!< Knots, kilometer / NMEA_TUD_KNOTS = knot
// #define NMEA_TUD_MILES      (1.609) //!< Miles, kilometer / NMEA_TUD_MILES = mile

// #define NMEA_NSATPACKS      (NMEA_MAXSAT / NMEA_SATINPACK)

// #define NMEA_EARTHRADIUS_KM (6378) //!< Earth's mean radius in km
// #define NMEA_EARTHRADIUS_M \
//     (NMEA_EARTHRADIUS_KM * 1000) //!< Earth's mean radius in m
// #define NMEA_EARTH_SEMIMAJORAXIS_M \
//     (6378137.0) //!< Earth's semi-major axis in m according WGS84
// #define NMEA_EARTH_SEMIMAJORAXIS_KM \
//     (NMEA_EARTHMAJORAXIS_KM /       \
//      1000) //!< Earth's semi-major axis in km according WGS 84
// #define NMEA_EARTH_FLATTENING \
//     (1 / 298.257223563)     //!< Earth's flattening according WGS 84
// #define NMEA_DOP_FACTOR (5) //!< Factor for translating DOP to meters

// static int _nmea_calc_crc(const char *buff, int buff_sz)
// {
//     int chsum = 0, it;

//     for (it = 0; it < buff_sz; ++it)
//         chsum ^= (int)buff[it];

//     return chsum;
// }

// static int _nmea_atoi(const char *str, size_t str_sz, int radix)
// {
//     char *tmp_ptr = 0;
//     char buff[NMEA_CONVSTR_BUF];
//     int res = 0;

//     if (str_sz < NMEA_CONVSTR_BUF) {
//         memcpy(&buff[0], str, str_sz);
//         buff[str_sz] = '\0';
//         res = strtol(&buff[0], &tmp_ptr, radix);
//     }

//     return res;
// }

// static double _nmea_atof(const char *str, int str_sz)
// {
//     char *tmp_ptr = 0;
//     char buff[NMEA_CONVSTR_BUF];
//     double res = 0;

//     if (str_sz < NMEA_CONVSTR_BUF) {
//         const char *oldlocale = setlocale(LC_NUMERIC, NULL);

//         memcpy(&buff[0], str, str_sz);
//         buff[str_sz] = '\0';
//         setlocale(LC_NUMERIC, "C");
//         res = strtod(&buff[0], &tmp_ptr);
//         setlocale(LC_NUMERIC, oldlocale);
//     }

//     return res;
// }

// static int _nmea_printf(char *buff, int buff_sz, const char *format, ...)
// {
//     int retval, add = 0;
//     va_list arg_ptr;

//     if (buff_sz <= 0)
//         return 0;

//     va_start(arg_ptr, format);

//     retval = NMEA_POSIX(vsnprintf)(buff, buff_sz, format, arg_ptr);

//     if (retval > 0) {
//         add = NMEA_POSIX(snprintf)(buff + retval, buff_sz - retval, "*%02x\r\n",
//                                    _nmea_calc_crc(buff + 1, retval - 1));
//     }

//     retval += add;

//     if (retval < 0 || retval > buff_sz) {
//         memset(buff, ' ', buff_sz);
//         retval = buff_sz;
//     }

//     va_end(arg_ptr);

//     return retval;
// }

// static int _nmea_scanf(const char *buff, int buff_sz, const char *format, ...)
// {
//     const char *beg_tok = 0;
//     const char *end_buf = buff + buff_sz;

//     va_list arg_ptr;
//     int tok_type = NMEA_TOKS_COMPARE;
//     int width = 0;
//     const char *beg_fmt = 0;
//     int snum = 0, unum = 0;

//     int tok_count = 0;
//     void *parg_target = 0;

//     va_start(arg_ptr, format);

//     for (; *format && buff < end_buf; ++format) {
//         switch (tok_type) {
//         case NMEA_TOKS_COMPARE:
//             if ('%' == *format)
//                 tok_type = NMEA_TOKS_PERCENT;
//             else if (*buff++ != *format)
//                 goto fail;
//             break;
//         case NMEA_TOKS_PERCENT:
//             width = 0;
//             beg_fmt = format;
//             tok_type = NMEA_TOKS_WIDTH;

//             FALLTHROUGH();

//         case NMEA_TOKS_WIDTH: {
//             if (isdigit(*format))
//                 break;

//             tok_type = NMEA_TOKS_TYPE;
//             if (format > beg_fmt)
//                 width = _nmea_atoi(beg_fmt, (int)(format - beg_fmt), 10);

//             FALLTHROUGH();
//         }

//         case NMEA_TOKS_TYPE:
//             beg_tok = buff;

//             if (!width && ('c' == *format || 'C' == *format) &&
//                 *buff != format[1])
//                 width = 1;

//             if (width) {
//                 if (buff + width <= end_buf)
//                     buff += width;
//                 else
//                     goto fail;
//             }
//             else {
//                 if (!format[1] || (0 == (buff = (char *)memchr(
//                                              buff, format[1], end_buf - buff))))
//                     buff = end_buf;
//             }

//             if (buff > end_buf)
//                 goto fail;

//             tok_type = NMEA_TOKS_COMPARE;
//             tok_count++;

//             parg_target = 0;
//             width = (int)(buff - beg_tok);

//             switch (*format) {
//             case 'c':
//             case 'C':
//                 parg_target = (void *)va_arg(arg_ptr, char *);
//                 if (width && 0 != (parg_target))
//                     *((char *)parg_target) = *beg_tok;
//                 break;
//             case 's':
//             case 'S':
//                 parg_target = (void *)va_arg(arg_ptr, char *);
//                 if (width && 0 != (parg_target)) {
//                     memcpy(parg_target, beg_tok, width);
//                     ((char *)parg_target)[width] = '\0';
//                 }
//                 break;
//             case 'f':
//             case 'g':
//             case 'G':
//             case 'e':
//             case 'E':
//                 parg_target = (void *)va_arg(arg_ptr, double *);
//                 if (width && 0 != (parg_target))
//                     *((double *)parg_target) = _nmea_atof(beg_tok, width);
//                 else if (width == 0 && 0 != (parg_target)) {
//                     *((double *)parg_target) = NAN;
//                 }
//                 break;
//             };

//             if (parg_target)
//                 break;
//             if (0 == (parg_target = (void *)va_arg(arg_ptr, int *)))
//                 break;
//             if (!width)
//                 break;

//             switch (*format) {
//             case 'd':
//             case 'i':
//                 snum = _nmea_atoi(beg_tok, width, 10);
//                 memcpy(parg_target, &snum, sizeof(int));
//                 break;
//             case 'u':
//                 unum = _nmea_atoi(beg_tok, width, 10);
//                 memcpy(parg_target, &unum, sizeof(unsigned int));
//                 break;
//             case 'x':
//             case 'X':
//                 unum = _nmea_atoi(beg_tok, width, 16);
//                 memcpy(parg_target, &unum, sizeof(unsigned int));
//                 break;
//             case 'o':
//                 unum = _nmea_atoi(beg_tok, width, 8);
//                 memcpy(parg_target, &unum, sizeof(unsigned int));
//                 break;
//             default:
//                 goto fail;
//             };

//             break;
//         };
//     }

// fail:

//     va_end(arg_ptr);

//     return tok_count;
// }

// static int _nmea_parse_time(const char *buff, int buff_sz, struct nmeaTIME *res)
// {
//     int success = 0;

//     switch (buff_sz) {
//     case sizeof("hhmmss") - 1:
//         success = (3 == _nmea_scanf(buff, buff_sz, "%2d%2d%2d", &(res->hour),
//                                     &(res->min), &(res->sec)));
//         break;
//     case sizeof("hhmmss.s") - 1:
//         success = (4 == _nmea_scanf(buff, buff_sz, "%2d%2d%2d.%d", &(res->hour),
//                                     &(res->min), &(res->sec), &(res->msec)));
//         res->msec = res->msec * 100; // tenths sec * 100 = thousandths
//         break;
//     case sizeof("hhmmss.ss") - 1:
//         success = (4 == _nmea_scanf(buff, buff_sz, "%2d%2d%2d.%d", &(res->hour),
//                                     &(res->min), &(res->sec), &(res->msec)));
//         res->msec = res->msec * 10; // hundredths sec * 10 = thousandths
//         break;
//     case sizeof("hhmmss.sss") - 1:
//         success = (4 == _nmea_scanf(buff, buff_sz, "%2d%2d%2d.%d", &(res->hour),
//                                     &(res->min), &(res->sec), &(res->msec)));
//         // already thousandths
//         break;
//     default:
//         LOG_ERROR("Parse of time error (format error)!");
//         success = 0;
//         break;
//     }

//     return (success ? 0 : -1);
// }

// void nmea_zero_GPGGA(struct nmeaGPGGA *pack)
// {
//     memset(pack, 0, sizeof(struct nmeaGPGGA));
//     nmea_time_now(&pack->utc);
//     pack->ns = 'N';
//     pack->ew = 'E';
//     pack->elv_units = 'M';
//     pack->diff_units = 'M';
// }

// void nmea_zero_GPGST(struct nmeaGPGST *pack)
// {
//     memset(pack, 0, sizeof(struct nmeaGPGST));
//     nmea_time_now(&pack->utc);
// }

// void nmea_zero_GPGSA(struct nmeaGPGSA *pack)
// {
//     memset(pack, 0, sizeof(struct nmeaGPGSA));
//     pack->fix_mode = 'A';
//     pack->fix_type = NMEA_FIX_BAD;
// }

// void nmea_zero_GPGSV(struct nmeaGPGSV *pack)
// {
//     memset(pack, 0, sizeof(struct nmeaGPGSV));
// }

// void nmea_zero_GPRMC(struct nmeaGPRMC *pack)
// {
//     memset(pack, 0, sizeof(struct nmeaGPRMC));
//     nmea_time_now(&pack->utc);
//     pack->status = 'V';
//     pack->ns = 'N';
//     pack->ew = 'E';
//     pack->declin_ew = 'E';
// }

// void nmea_zero_GPVTG(struct nmeaGPVTG *pack)
// {
//     memset(pack, 0, sizeof(struct nmeaGPVTG));
//     pack->dir_t = 'T';
//     pack->dec_m = 'M';
//     pack->spn_n = 'N';
//     pack->spk_k = 'K';
// }

// /**
//  * \brief Define packet type by header (nmeaPACKTYPE).
//  * @param buff a constant character pointer of packet buffer.
//  * @param buff_sz buffer size.
//  * @return The defined packet type
//  * @see nmeaPACKTYPE
//  */
// int nmea_pack_type(const char *buff, int buff_sz)
// {
//     static const char *P_HEADS[] = {
//         "GPGGA", "GPGSA", "GPGSV", "GPRMC", "GPVTG",
//         "HCHDG", "HCHDT", "GNRMC", "GPGST",
//     };

//     // BUFFER_SIZE = size(P_HEADS) - 1;
//     int buffer_size = 8;

//     assert(buff);

//     if (buff_sz < buffer_size)
//         return GPNON;
//     else if (0 == memcmp(buff, P_HEADS[0], buffer_size))
//         return GPGGA;
//     else if (0 == memcmp(buff, P_HEADS[1], buffer_size))
//         return GPGSA;
//     else if (0 == memcmp(buff, P_HEADS[2], buffer_size))
//         return GPGSV;
//     else if (0 == memcmp(buff, P_HEADS[3], buffer_size))
//         return GPRMC;
//     else if (0 == memcmp(buff, P_HEADS[4], buffer_size))
//         return GPVTG;
//     else if (0 == memcmp(buff, P_HEADS[5], buffer_size))
//         return HCHDG;
//     else if (0 == memcmp(buff, P_HEADS[6], buffer_size))
//         return HCHDT;
//     else if (0 == memcmp(buff, P_HEADS[7], buffer_size))
//         return GPRMC;
//     else if (0 == memcmp(buff, P_HEADS[8], buffer_size))
//         return GPGST;

//     return GPNON;
// }

// /**
//  * \brief Find tail of packet ("\r\n") in buffer and check control sum (CRC).
//  * @param buff a constant character pointer of packets buffer.
//  * @param buff_sz buffer size.
//  * @param res_crc a integer pointer for return CRC of packet (must be defined).
//  * @return Number of bytes to packet tail.
//  */
// int nmea_find_tail(const char *buff, int buff_sz, int *res_crc)
// {
//     static const int tail_sz = 3 /* *[CRC] */ + 2 /* \r\n */;

//     const char *end_buff = buff + buff_sz;
//     int nread = 0;
//     int crc = 0;

//     assert(buff && res_crc);

//     *res_crc = -1;

//     for (; buff < end_buff; ++buff, ++nread) {
//         if (('$' == *buff) && nread) {
//             buff = 0;
//             break;
//         }
//         else if ('*' == *buff) {
//             if (buff + tail_sz <= end_buff && '\r' == buff[3] &&
//                 '\n' == buff[4]) {
//                 *res_crc = _nmea_atoi(buff + 1, 2, 16);
//                 nread = buff_sz - (int)(end_buff - (buff + tail_sz));
//                 if (*res_crc != crc) {
//                     *res_crc = -1;
//                     buff = 0;
//                 }
//             }

//             break;
//         }
//         else if (nread)
//             crc ^= (int)*buff;
//     }

//     if (*res_crc < 0 && buff)
//         nread = 0;

//     return nread;
// }

// /**
//  * \brief Parse GGA packet from buffer.
//  * @param buff a constant character pointer of packet buffer.
//  * @param buff_sz buffer size.
//  * @param pack a pointer of packet which will filled by function.
//  * @return 1 (true) - if parsed successfully or 0 (false) - if fail.
//  */
// int nmea_parse_GPGGA(const char *buff, int buff_sz, struct nmeaGPGGA *pack)
// {
//     char time_buff[NMEA_TIMEPARSE_BUF];

//     assert(buff && pack);

//     memset(pack, 0, sizeof(struct nmeaGPGGA));

//     char type;

//     if (15 != _nmea_scanf(
//                   buff, buff_sz,
//                   "$G%CGGA,%s,%f,%C,%f,%C,%d,%d,%f,%f,%C,%f,%C,%f,%d*", &(type),
//                   &(time_buff[0]), &(pack->lat), &(pack->ns), &(pack->lon),
//                   &(pack->ew), &(pack->sig), &(pack->satinuse), &(pack->HDOP),
//                   &(pack->elv), &(pack->elv_units), &(pack->diff),
//                   &(pack->diff_units), &(pack->dgps_age), &(pack->dgps_sid))) {
//         LOG_ERROR("G?GGA parse error!");
//         return 0;
//     }

//     if (type != 'P' && type != 'N') {
//         LOG_ERROR("G?GGA invalid type ");
//         return 0;
//     }

//     if (0 != _nmea_parse_time(&time_buff[0], (int)strlen(&time_buff[0]),
//                               &(pack->utc))) {
//         LOG_ERROR("GPGGA time parse error!");
//         return 0;
//     }

//     return 1;
// }

// /**
//  * \brief Parse GST packet from buffer.
//  * @param buff a constant character pointer of packet buffer.
//  * @param buff_sz buffer size.
//  * @param pack a pointer of packet which will filled by function.
//  * @return 1 (true) - if parsed successfully or 0 (false) - if fail.
//  */
// int nmea_parse_GPGST(const char *buff, int buff_sz, struct nmeaGPGST *pack)
// {
//     char time_buff[NMEA_TIMEPARSE_BUF];

//     assert(buff && pack);

//     memset(pack, 0, sizeof(struct nmeaGPGST));

//     char type;

//     if (9 != _nmea_scanf(buff, buff_sz, "$G%CGST,%s,%f,%f,%f,%f,%f,%f,%f*",
//                          &(type), &(time_buff[0]), &(pack->rms_pr),
//                          &(pack->err_major), &(pack->err_minor),
//                          &(pack->err_ori), &(pack->sig_lat), &(pack->sig_lon),
//                          &(pack->sig_alt))) {
//         LOG_ERROR("G?GST parse error!");
//         return 0;
//     }

//     if (type != 'P' && type != 'N') {
//         LOG_ERROR("G?GST invalid type ");
//         return 0;
//     }

//     if (0 != _nmea_parse_time(&time_buff[0], (int)strlen(&time_buff[0]),
//                               &(pack->utc))) {
//         LOG_ERROR("G?GST time parse error!");
//         return 0;
//     }

//     return 1;
// }

// /**
//  * \brief Parse GSA packet from buffer.
//  * @param buff a constant character pointer of packet buffer.
//  * @param buff_sz buffer size.
//  * @param pack a pointer of packet which will filled by function.
//  * @return 1 (true) - if parsed successfully or 0 (false) - if fail.
//  */
// int nmea_parse_GPGSA(const char *buff, int buff_sz, struct nmeaGPGSA *pack)
// {
//     assert(buff && pack);

//     memset(pack, 0, sizeof(struct nmeaGPGSA));

//     if (18 != _nmea_scanf(
//                   buff, buff_sz,
//                   "$G%CGSA,%C,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%f,%f,%f*",
//                   &(pack->pack_type), &(pack->fix_mode), &(pack->fix_type),
//                   &(pack->sat_prn[0]), &(pack->sat_prn[1]), &(pack->sat_prn[2]),
//                   &(pack->sat_prn[3]), &(pack->sat_prn[4]), &(pack->sat_prn[5]),
//                   &(pack->sat_prn[6]), &(pack->sat_prn[7]), &(pack->sat_prn[8]),
//                   &(pack->sat_prn[9]), &(pack->sat_prn[10]),
//                   &(pack->sat_prn[11]), &(pack->PDOP), &(pack->HDOP),
//                   &(pack->VDOP))) {
//         LOG_ERROR("G?GSA parse error!");
//         return 0;
//     }

//     if (pack->pack_type != 'P' && pack->pack_type != 'N' &&
//         pack->pack_type != 'L') {
//         LOG_ERROR("G?GSA invalid type ");
//         return 0;
//     }

//     return 1;
// }

// /**
//  * \brief Parse GSV packet from buffer.
//  * @param buff a constant character pointer of packet buffer.
//  * @param buff_sz buffer size.
//  * @param pack a pointer of packet which will filled by function.
//  * @return 1 (true) - if parsed successfully or 0 (false) - if fail.
//  */
// int nmea_parse_GPGSV(const char *buff, int buff_sz, struct nmeaGPGSV *pack)
// {
//     int nsen, nsat;

//     assert(buff && pack);

//     memset(pack, 0, sizeof(struct nmeaGPGSV));

//     nsen = _nmea_scanf(buff, buff_sz,
//                        "$G%CGSV,%d,%d,%d,"
//                        "%d,%d,%d,%d,"
//                        "%d,%d,%d,%d,"
//                        "%d,%d,%d,%d,"
//                        "%d,%d,%d,%d*",
//                        &(pack->pack_type), &(pack->pack_count),
//                        &(pack->pack_index), &(pack->sat_count),
//                        &(pack->sat_data[0].id), &(pack->sat_data[0].elv),
//                        &(pack->sat_data[0].azimuth), &(pack->sat_data[0].sig),
//                        &(pack->sat_data[1].id), &(pack->sat_data[1].elv),
//                        &(pack->sat_data[1].azimuth), &(pack->sat_data[1].sig),
//                        &(pack->sat_data[2].id), &(pack->sat_data[2].elv),
//                        &(pack->sat_data[2].azimuth), &(pack->sat_data[2].sig),
//                        &(pack->sat_data[3].id), &(pack->sat_data[3].elv),
//                        &(pack->sat_data[3].azimuth), &(pack->sat_data[3].sig));

//     nsat = (pack->pack_index - 1) * NMEA_SATINPACK;
//     nsat = (nsat + NMEA_SATINPACK > pack->sat_count) ? pack->sat_count - nsat
//                                                      : NMEA_SATINPACK;
//     nsat = nsat * 4 + 3 /* first three sentence`s */;

//     if (nsen - 1 < nsat || nsen - 1 > (NMEA_SATINPACK * 4 + 3)) {
//         LOG_ERROR("G?GSV parse error!");
//         return 0;
//     }

//     if (pack->pack_type != 'P' && pack->pack_type != 'N' &&
//         pack->pack_type != 'L' && pack->pack_type != 'A' &&
//         pack->pack_type != 'B' && pack->pack_type != 'Q') {
//         LOG_ERROR("G?GSV invalid type ");
//         return 0;
//     }

//     return 1;
// }

// /**
//  * \brief Parse RMC packet from buffer.
//  * @param buff a constant character pointer of packet buffer.
//  * @param buff_sz buffer size.
//  * @param pack a pointer of packet which will filled by function.
//  * @return 1 (true) - if parsed successfully or 0 (false) - if fail.
//  */
// int nmea_parse_GPRMC(const char *buff, int buff_sz, struct nmeaGPRMC *pack)
// {
//     int nsen;
//     char type;
//     char time_buff[NMEA_TIMEPARSE_BUF];

//     assert(buff && pack);

//     memset(pack, 0, sizeof(struct nmeaGPRMC));

//     nsen = _nmea_scanf(buff, buff_sz,
//                        "$G%CRMC,%s,%C,%f,%C,%f,%C,%f,%f,%2d%2d%2d,%f,%C,%C,%C*",
//                        &(type), &(time_buff[0]), &(pack->status), &(pack->lat),
//                        &(pack->ns), &(pack->lon), &(pack->ew), &(pack->speed),
//                        &(pack->direction), &(pack->utc.day), &(pack->utc.mon),
//                        &(pack->utc.year), &(pack->declination),
//                        &(pack->declin_ew), &(pack->mode), &(pack->navstatus));

//     if (nsen < 14 || nsen > 16) {
//         LOG_ERROR("G?RMC parse error!");
//         return 0;
//     }

//     if (type != 'P' && type != 'N') {
//         LOG_ERROR("G?RMC invalid type ");
//         return 0;
//     }

//     if (0 != _nmea_parse_time(&time_buff[0], (int)strlen(&time_buff[0]),
//                               &(pack->utc))) {
//         LOG_ERROR("GPRMC time parse error!");
//         return 0;
//     }

//     if (pack->utc.year < 90)
//         pack->utc.year += 100;
//     pack->utc.mon -= 1;

//     return 1;
// }

// /**
//  * \brief Parse HDT packet from buffer.
//  * @param buff a constant character pointer of packet buffer.
//  * @param buff_sz buffer size.
//  * @param pack a pointer of packet which will filled by function.
//  * @return 1 (true) - if parsed successfully or 0 (false) - if fail.
//  */
// int nmea_parse_GPHDT(const char *buff, int buff_sz, struct nmeaGPHDT *pack)
// {
//     assert(buff && pack);

//     memset(pack, 0, sizeof(struct nmeaGPHDT));

//     char type;
//     char talker_id;

//     if (3 != _nmea_scanf(buff, buff_sz, "$G%CHDT,%f,%C*", &(talker_id),
//                          &(pack->heading), &(type))) {
//         LOG_ERROR("G?HDT parse error!");
//         return 0;
//     }

//     if (talker_id != 'P' && talker_id != 'N') {
//         LOG_ERROR("G?HDT invalid type ");
//         return 0;
//     }

//     if (type != 'T') {
//         LOG_ERROR("G?HDT invalid type ");
//         return 0;
//     }

//     return 1;
// }

// /**
//  * \brief Parse VTG packet from buffer.
//  * @param buff a constant character pointer of packet buffer.
//  * @param buff_sz buffer size.
//  * @param pack a pointer of packet which will filled by function.
//  * @return 1 (true) - if parsed successfully or 0 (false) - if fail.
//  */
// int nmea_parse_GPVTG(const char *buff, int buff_sz, struct nmeaGPVTG *pack)
// {
//     assert(buff && pack);

//     memset(pack, 0, sizeof(struct nmeaGPVTG));

//     char type;

//     if (9 != _nmea_scanf(buff, buff_sz, "$G%CVTG,%f,%C,%f,%C,%f,%C,%f,%C*",
//                          &type, &(pack->dir), &(pack->dir_t), &(pack->dec),
//                          &(pack->dec_m), &(pack->spn), &(pack->spn_n),
//                          &(pack->spk), &(pack->spk_k))) {
//         LOG_ERROR("G?VTG parse error!");
//         return 0;
//     }

//     if (type != 'P' && type != 'N') {
//         LOG_ERROR("G?VTG invalid type ");
//         return 0;
//     }

//     if (pack->dir_t != 'T' || pack->dec_m != 'M' || pack->spn_n != 'N' ||
//         pack->spk_k != 'K') {
//         LOG_ERROR("G?VTG parse error (format error)!");
//         return 0;
//     }

//     return 1;
// }

// /**
//  * \brief Parse HCHDG packet from buffer.
//  * @param buff a constant character pointer of packet buffer.
//  * @param buff_sz buffer size.
//  * @param pack a pointer of packet which will filled by function.
//  * @return 1 (true) - if parsed successfully or 0 (false) - if fail.
//  */
// int nmea_parse_HCHDG(const char *buff, int buff_sz, struct nmeaHCHDG *pack)
// {
//     assert(buff && pack);

//     memset(pack, 0, sizeof(struct nmeaHCHDG));

//     if (5 != _nmea_scanf(buff, buff_sz, "$HCHDG,%f,%f,%C,%f,%C*",
//                          &(pack->mag_heading), &(pack->mag_deviation),
//                          &(pack->ew_deviation), &(pack->mag_variation),
//                          &(pack->ew_variation))) {
//         LOG_ERROR("HCHDG parse error!");
//         return 0;
//     }

//     if (pack->ew_deviation != 'E' && pack->ew_deviation != 'W') {
//         LOG_ERROR("HCHDG invalid deviation direction");
//         return 0;
//     }

//     if (pack->ew_variation != 'E' && pack->ew_variation != 'W') {
//         LOG_ERROR("HCHDG invalid variation direction");
//         return 0;
//     }

//     return 1;
// }

// /**
//  * \brief Parse HDT packet from buffer.
//  * @param buff a constant character pointer of packet buffer.
//  * @param buff_sz buffer size.
//  * @param pack a pointer of packet which will filled by function.
//  * @return 1 (true) - if parsed successfully or 0 (false) - if fail.
//  */
// int nmea_parse_HCHDT(const char *buff, int buff_sz, struct nmeaHCHDT *pack)
// {
//     assert(buff && pack);

//     memset(pack, 0, sizeof(struct nmeaHCHDT));

//     if (2 != _nmea_scanf(buff, buff_sz, "$HCHDT,%f,%C*", &(pack->direction),
//                          &(pack->t_flag))) {
//         LOG_ERROR("HCHDT parse error!");
//         return 0;
//     }

//     return 1;
// }

// /**
//  * \brief Fill nmeaINFO structure by GGA packet data.
//  * @param pack a pointer of packet structure.
//  * @param info a pointer of summary information structure.
//  */
// void nmea_GPGGA2info(struct nmeaGPGGA *pack, struct nmeaINFO *info)
// {
//     assert(pack && info);

//     info->utc.hour = pack->utc.hour;
//     info->utc.min = pack->utc.min;
//     info->utc.sec = pack->utc.sec;
//     info->utc.msec = pack->utc.msec;
//     info->sig = pack->sig;
//     info->HDOP = pack->HDOP;
//     info->elv = pack->elv;
//     info->lat = ((pack->ns == 'N') ? pack->lat : -(pack->lat));
//     info->lon = ((pack->ew == 'E') ? pack->lon : -(pack->lon));
//     info->smask |= GPGGA;
// }

// /**
//  * \brief Fill nmeaINFO structure by GST packet data.
//  * @param pack a pointer of packet structure.
//  * @param info a pointer of summary information structure.
//  */
// void nmea_GPGST2info(struct nmeaGPGST *pack, struct nmeaINFO *info)
// {
//     assert(pack && info);

//     info->utc.hour = pack->utc.hour;
//     info->utc.min = pack->utc.min;
//     info->utc.sec = pack->utc.sec;
//     info->utc.msec = pack->utc.msec;
//     info->rms_pr = pack->rms_pr;
//     info->err_major = pack->err_major;
//     info->err_minor = pack->err_minor;
//     info->err_ori = pack->err_ori;
//     info->sig_lat = pack->sig_lat;
//     info->sig_lon = pack->sig_lon;
//     info->sig_alt = pack->sig_alt;
//     info->smask |= GPGST;
// }

// /**
//  * \brief Fill nmeaINFO structure by GSA packet data.
//  * @param pack a pointer of packet structure.
//  * @param info a pointer of summary information structure.
//  */
// void nmea_GPGSA2info(struct nmeaGPGSA *pack, struct nmeaINFO *info)
// {
//     int i, j, nuse = 0;

//     assert(pack && info);

//     info->fix = pack->fix_type;
//     info->PDOP = pack->PDOP;
//     info->HDOP = pack->HDOP;
//     info->VDOP = pack->VDOP;

//     for (i = 0; i < NMEA_MAXSAT; ++i) {
//         for (j = 0; j < info->satinfo.inview; ++j) {
//             if (pack->sat_prn[i] &&
//                 pack->sat_prn[i] == info->satinfo.sat[j].id) {
//                 info->satinfo.sat[j].in_use = 1;
//                 nuse++;
//             }
//         }
//     }

//     info->satinfo.inuse = nuse;
//     info->smask |= GPGSA;
// }

// /**
//  * \brief Fill nmeaINFO structure by GSV packet data.
//  * @param pack a pointer of packet structure.
//  * @param info a pointer of summary information structure.
//  */
// void nmea_GPGSV2info(struct nmeaGPGSV *pack, struct nmeaINFO *info)
// {
//     int isat, isi, nsat;

//     assert(pack && info);

//     if (pack->pack_index > pack->pack_count ||
//         pack->pack_index * NMEA_SATINPACK > NMEA_MAXSAT)
//         return;

//     if (pack->pack_index < 1)
//         pack->pack_index = 1;

//     info->satinfo.inview = pack->sat_count;

//     nsat = (pack->pack_index - 1) * NMEA_SATINPACK;
//     nsat = (nsat + NMEA_SATINPACK > pack->sat_count) ? pack->sat_count - nsat
//                                                      : NMEA_SATINPACK;

//     for (isat = 0; isat < nsat; ++isat) {
//         isi = (pack->pack_index - 1) * NMEA_SATINPACK + isat;
//         info->satinfo.sat[isi].id = pack->sat_data[isat].id;
//         info->satinfo.sat[isi].elv = pack->sat_data[isat].elv;
//         info->satinfo.sat[isi].azimuth = pack->sat_data[isat].azimuth;
//         info->satinfo.sat[isi].sig = pack->sat_data[isat].sig;
//     }

//     info->smask |= GPGSV;
// }

// /**
//  * \brief Fill nmeaINFO structure by RMC packet data.
//  * @param pack a pointer of packet structure.
//  * @param info a pointer of summary information structure.
//  */
// void nmea_GPRMC2info(struct nmeaGPRMC *pack, struct nmeaINFO *info)
// {
//     assert(pack && info);

//     if ('A' == pack->status) {
//         if (NMEA_SIG_BAD == info->sig)
//             info->sig = NMEA_SIG_MID;
//         if (NMEA_FIX_BAD == info->fix)
//             info->fix = NMEA_FIX_2D;
//     }
//     else if ('V' == pack->status) {
//         info->sig = NMEA_SIG_BAD;
//         info->fix = NMEA_FIX_BAD;
//     }

//     info->utc = pack->utc;
//     info->lat = ((pack->ns == 'N') ? pack->lat : -(pack->lat));
//     info->lon = ((pack->ew == 'E') ? pack->lon : -(pack->lon));
//     info->speed = pack->speed * NMEA_TUD_KNOTS;
//     info->direction = pack->direction;
//     info->smask |= GPRMC;
// }

// /**
//  * \brief Fill nmeaINFO structure by VTG packet data.
//  * @param pack a pointer of packet structure.
//  * @param info a pointer of summary information structure.
//  */
// void nmea_GPVTG2info(struct nmeaGPVTG *pack, struct nmeaINFO *info)
// {
//     assert(pack && info);

//     info->direction = pack->dir;
//     info->declination = pack->dec;
//     info->speed = pack->spk;
//     info->smask |= GPVTG;
// }

// /* ------------------------------------------ nmea math
//  * ----------------------------------------- */

// /**
//  * \fn nmea_degree2radian
//  * \brief Convert degree to radian
//  */
// double nmea_degree2radian(double val)
// {
//     return (val * (M_PI / 180));
// }

// /**
//  * \fn nmea_radian2degree
//  * \brief Convert radian to degree
//  */
// double nmea_radian2degree(double val)
// {
//     return (val / (M_PI / 180));
// }

// /**
//  * \brief Convert NDEG (NMEA degree) to fractional degree
//  */
// double nmea_ndeg2degree(double val)
// {
//     double deg = ((int)(val / 100));
//     val = deg + (val - deg * 100) / 60;
//     return val;
// }

// /**
//  * \brief Convert fractional degree to NDEG (NMEA degree)
//  */
// double nmea_degree2ndeg(double val)
// {
//     double int_part;
//     double fra_part;
//     fra_part = modf(val, &int_part);
//     val = int_part * 100 + fra_part * 60;
//     return val;
// }

// /**
//  * \fn nmea_ndeg2radian
//  * \brief Convert NDEG (NMEA degree) to radian
//  */
// double nmea_ndeg2radian(double val)
// {
//     return nmea_degree2radian(nmea_ndeg2degree(val));
// }

// /**
//  * \fn nmea_radian2ndeg
//  * \brief Convert radian to NDEG (NMEA degree)
//  */
// double nmea_radian2ndeg(double val)
// {
//     return nmea_degree2ndeg(nmea_radian2degree(val));
// }

// /**
//  * \brief Calculate PDOP (Position Dilution Of Precision) factor
//  */
// double nmea_calc_pdop(double hdop, double vdop)
// {
//     return sqrt(pow(hdop, 2) + pow(vdop, 2));
// }

// double nmea_dop2meters(double dop)
// {
//     return (dop * NMEA_DOP_FACTOR);
// }

// double nmea_meters2dop(double meters)
// {
//     return (meters / NMEA_DOP_FACTOR);
// }

// /**
//  * \brief Calculate distance between two points
//  * \return Distance in meters
//  */
// double
// nmea_distance(const struct nmeaPOS *from_pos, //!< From position in radians
//               const struct nmeaPOS *to_pos    //!< To position in radians
// )
// {
//     double dist = ((double)NMEA_EARTHRADIUS_M) *
//                   acos(sin(to_pos->lat) * sin(from_pos->lat) +
//                        cos(to_pos->lat) * cos(from_pos->lat) *
//                            cos(to_pos->lon - from_pos->lon));
//     return dist;
// }

// /**
//  * \brief Calculate distance between two points
//  * This function uses an algorithm for an oblate spheroid earth model.
//  * The algorithm is described here:
//  * http://www.ngs.noaa.gov/PUBS_LIB/inverse.pdf
//  * \return Distance in meters
//  */
// double nmea_distance_ellipsoid(
//     const struct nmeaPOS *from_pos, //!< From position in radians
//     const struct nmeaPOS *to_pos,   //!< To position in radians
//     double *from_azimuth, //!< (O) azimuth at "from" position in radians
//     double *to_azimuth    //!< (O) azimuth at "to" position in radians
// )
// {
//     /* All variables */
//     double f, a, b, sqr_a, sqr_b;
//     double L, phi1, phi2, U1, U2, sin_U1, sin_U2, cos_U1, cos_U2;
//     double sigma, sin_sigma, cos_sigma, cos_2_sigmam, sqr_cos_2_sigmam,
//         sqr_cos_alpha, lambda, sin_lambda, cos_lambda, delta_lambda;
//     int remaining_steps;
//     double sqr_u, A, B, delta_sigma;

//     /* Check input */
//     assert(from_pos != 0);
//     assert(to_pos != 0);

//     if ((from_pos->lat == to_pos->lat) && (from_pos->lon == to_pos->lon)) {
//         /* Identical points */
//         if (from_azimuth != 0)
//             *from_azimuth = 0;
//         if (to_azimuth != 0)
//             *to_azimuth = 0;
//         return 0;
//     } /* Identical points */

//     /* Earth geometry */
//     f = NMEA_EARTH_FLATTENING;
//     a = NMEA_EARTH_SEMIMAJORAXIS_M;
//     b = (1 - f) * a;
//     sqr_a = a * a;
//     sqr_b = b * b;

//     /* Calculation */
//     L = to_pos->lon - from_pos->lon;
//     phi1 = from_pos->lat;
//     phi2 = to_pos->lat;
//     U1 = atan((1 - f) * tan(phi1));
//     U2 = atan((1 - f) * tan(phi2));
//     sin_U1 = sin(U1);
//     sin_U2 = sin(U2);
//     cos_U1 = cos(U1);
//     cos_U2 = cos(U2);

//     /* Initialize iteration */
//     sigma = 0;
//     sin_sigma = sin(sigma);
//     cos_sigma = cos(sigma);
//     cos_2_sigmam = 0;
//     sqr_cos_2_sigmam = cos_2_sigmam * cos_2_sigmam;
//     sqr_cos_alpha = 0;
//     lambda = L;
//     sin_lambda = sin(lambda);
//     cos_lambda = cos(lambda);
//     delta_lambda = lambda;
//     remaining_steps = 20;

//     while ((delta_lambda > 1e-12) && (remaining_steps > 0)) {
//         /* Iterate */
//         /* Variables */
//         double tmp1, tmp2, sin_alpha, cos_alpha, C, lambda_prev;

//         /* Calculation */
//         tmp1 = cos_U2 * sin_lambda;
//         tmp2 = cos_U1 * sin_U2 - sin_U1 * cos_U2 * cos_lambda;
//         sin_sigma = sqrt(tmp1 * tmp1 + tmp2 * tmp2);
//         cos_sigma = sin_U1 * sin_U2 + cos_U1 * cos_U2 * cos_lambda;
//         sin_alpha = cos_U1 * cos_U2 * sin_lambda / sin_sigma;
//         cos_alpha = cos(asin(sin_alpha));
//         sqr_cos_alpha = cos_alpha * cos_alpha;
//         cos_2_sigmam = cos_sigma - 2 * sin_U1 * sin_U2 / sqr_cos_alpha;
//         sqr_cos_2_sigmam = cos_2_sigmam * cos_2_sigmam;
//         C = f / 16 * sqr_cos_alpha * (4 + f * (4 - 3 * sqr_cos_alpha));
//         lambda_prev = lambda;
//         sigma = asin(sin_sigma);
//         lambda =
//             L + (1 - C) * f * sin_alpha *
//                     (sigma + C * sin_sigma *
//                                  (cos_2_sigmam +
//                                   C * cos_sigma * (-1 + 2 * sqr_cos_2_sigmam)));
//         delta_lambda = lambda_prev - lambda;
//         if (delta_lambda < 0)
//             delta_lambda = -delta_lambda;
//         sin_lambda = sin(lambda);
//         cos_lambda = cos(lambda);
//         remaining_steps--;
//     } /* Iterate */

//     /* More calculation  */
//     sqr_u = sqr_cos_alpha * (sqr_a - sqr_b) / sqr_b;
//     A = 1 +
//         sqr_u / 16384 * (4096 + sqr_u * (-768 + sqr_u * (320 - 175 * sqr_u)));
//     B = sqr_u / 1024 * (256 + sqr_u * (-128 + sqr_u * (74 - 47 * sqr_u)));
//     delta_sigma = B * sin_sigma *
//                   (cos_2_sigmam + B / 4 *
//                                       (cos_sigma * (-1 + 2 * sqr_cos_2_sigmam) -
//                                        B / 6 * cos_2_sigmam *
//                                            (-3 + 4 * sin_sigma * sin_sigma) *
//                                            (-3 + 4 * sqr_cos_2_sigmam)));

//     /* Calculate result */
//     if (from_azimuth != 0) {
//         double tan_alpha_1 = cos_U2 * sin_lambda /
//                              (cos_U1 * sin_U2 - sin_U1 * cos_U2 * cos_lambda);
//         *from_azimuth = atan(tan_alpha_1);
//     }
//     if (to_azimuth != 0) {
//         double tan_alpha_2 = cos_U1 * sin_lambda /
//                              (-sin_U1 * cos_U2 + cos_U1 * sin_U2 * cos_lambda);
//         *to_azimuth = atan(tan_alpha_2);
//     }

//     return b * A * (sigma - delta_sigma);
// }

// /**
//  * \brief Horizontal move of point position
//  */
// int nmea_move_horz(
//     const struct nmeaPOS *start_pos, //!< Start position in radians
//     struct nmeaPOS *end_pos,         //!< Result position in radians
//     double azimuth,                  //!< Azimuth (degree) [0, 359]
//     double distance                  //!< Distance (km)
// )
// {
//     struct nmeaPOS p1 = *start_pos;
//     int RetVal = 1;

//     distance /=
//         NMEA_EARTHRADIUS_KM; /* Angular distance covered on earth's surface */
//     azimuth = nmea_degree2radian(azimuth);

//     end_pos->lat = asin(sin(p1.lat) * cos(distance) +
//                         cos(p1.lat) * sin(distance) * cos(azimuth));
//     end_pos->lon =
//         p1.lon + atan2(sin(azimuth) * sin(distance) * cos(p1.lat),
//                        cos(distance) - sin(p1.lat) * sin(end_pos->lat));

//     if (NMEA_POSIX(isnan)(end_pos->lat) || NMEA_POSIX(isnan)(end_pos->lon)) {
//         end_pos->lat = 0;
//         end_pos->lon = 0;
//         RetVal = 0;
//     }

//     return RetVal;
// }

// /**
//  * \brief Horizontal move of point position
//  * This function uses an algorithm for an oblate spheroid earth model.
//  * The algorithm is described here:
//  * http://www.ngs.noaa.gov/PUBS_LIB/inverse.pdf
//  */
// int nmea_move_horz_ellipsoid(
//     const struct nmeaPOS *start_pos, //!< Start position in radians
//     struct nmeaPOS *end_pos,         //!< (O) Result position in radians
//     double azimuth,                  //!< Azimuth in radians
//     double distance,                 //!< Distance (km)
//     double *end_azimuth              //!< (O) Azimuth at end position in radians
// )
// {
//     /* Variables */
//     double f, a, b, sqr_a, sqr_b;
//     double phi1, tan_U1, sin_U1, cos_U1, s, alpha1, sin_alpha1, cos_alpha1;
//     double sigma1, sin_alpha, sqr_cos_alpha, sqr_u, A, B;
//     double sigma_initial, sigma, sigma_prev, sin_sigma, cos_sigma, cos_2_sigmam,
//         sqr_cos_2_sigmam, delta_sigma;
//     int remaining_steps;
//     double tmp1, phi2, lambda, C, L;

//     /* Check input */
//     assert(start_pos != 0);
//     assert(end_pos != 0);

//     if (fabs(distance) < 1e-12) {
//         /* No move */
//         *end_pos = *start_pos;
//         if (end_azimuth != 0)
//             *end_azimuth = azimuth;
//         return !(NMEA_POSIX(isnan)(end_pos->lat) ||
//                  NMEA_POSIX(isnan)(end_pos->lon));
//     } /* No move */

//     /* Earth geometry */
//     f = NMEA_EARTH_FLATTENING;
//     a = NMEA_EARTH_SEMIMAJORAXIS_M;
//     b = (1 - f) * a;
//     sqr_a = a * a;
//     sqr_b = b * b;

//     /* Calculation */
//     phi1 = start_pos->lat;
//     tan_U1 = (1 - f) * tan(phi1);
//     cos_U1 = 1 / sqrt(1 + tan_U1 * tan_U1);
//     sin_U1 = tan_U1 * cos_U1;
//     s = distance;
//     alpha1 = azimuth;
//     sin_alpha1 = sin(alpha1);
//     cos_alpha1 = cos(alpha1);
//     sigma1 = atan2(tan_U1, cos_alpha1);
//     sin_alpha = cos_U1 * sin_alpha1;
//     sqr_cos_alpha = 1 - sin_alpha * sin_alpha;
//     sqr_u = sqr_cos_alpha * (sqr_a - sqr_b) / sqr_b;
//     A = 1 +
//         sqr_u / 16384 * (4096 + sqr_u * (-768 + sqr_u * (320 - 175 * sqr_u)));
//     B = sqr_u / 1024 * (256 + sqr_u * (-128 + sqr_u * (74 - 47 * sqr_u)));

//     /* Initialize iteration */
//     sigma_initial = s / (b * A);
//     sigma = sigma_initial;
//     sin_sigma = sin(sigma);
//     cos_sigma = cos(sigma);
//     cos_2_sigmam = cos(2 * sigma1 + sigma);
//     sqr_cos_2_sigmam = cos_2_sigmam * cos_2_sigmam;
//     delta_sigma = 0;
//     sigma_prev = 2 * M_PI;
//     remaining_steps = 20;

//     while ((fabs(sigma - sigma_prev) > 1e-12) && (remaining_steps > 0)) {
//         /* Iterate */
//         cos_2_sigmam = cos(2 * sigma1 + sigma);
//         sqr_cos_2_sigmam = cos_2_sigmam * cos_2_sigmam;
//         sin_sigma = sin(sigma);
//         cos_sigma = cos(sigma);
//         delta_sigma =
//             B * sin_sigma *
//             (cos_2_sigmam +
//              B / 4 *
//                  (cos_sigma * (-1 + 2 * sqr_cos_2_sigmam) -
//                   B / 6 * cos_2_sigmam * (-3 + 4 * sin_sigma * sin_sigma) *
//                       (-3 + 4 * sqr_cos_2_sigmam)));
//         sigma_prev = sigma;
//         sigma = sigma_initial + delta_sigma;
//         remaining_steps--;
//     } /* Iterate */

//     /* Calculate result */
//     tmp1 = (sin_U1 * sin_sigma - cos_U1 * cos_sigma * cos_alpha1);
//     phi2 = atan2(sin_U1 * cos_sigma + cos_U1 * sin_sigma * cos_alpha1,
//                  (1 - f) * sqrt(sin_alpha * sin_alpha + tmp1 * tmp1));
//     lambda = atan2(sin_sigma * sin_alpha1,
//                    cos_U1 * cos_sigma - sin_U1 * sin_sigma * cos_alpha1);
//     C = f / 16 * sqr_cos_alpha * (4 + f * (4 - 3 * sqr_cos_alpha));
//     L = lambda -
//         (1 - C) * f * sin_alpha *
//             (sigma +
//              C * sin_sigma *
//                  (cos_2_sigmam + C * cos_sigma * (-1 + 2 * sqr_cos_2_sigmam)));

//     /* Result */
//     end_pos->lon = start_pos->lon + L;
//     end_pos->lat = phi2;
//     if (end_azimuth != 0) {
//         *end_azimuth = atan2(sin_alpha, -sin_U1 * sin_sigma +
//                                             cos_U1 * cos_sigma * cos_alpha1);
//     }
//     return !(NMEA_POSIX(isnan)(end_pos->lat) ||
//              NMEA_POSIX(isnan)(end_pos->lon));
// }

// /**
//  * \brief Convert position from INFO to radians position
//  */
// void nmea_info2pos(const struct nmeaINFO *info, struct nmeaPOS *pos)
// {
//     pos->lat = nmea_ndeg2radian(info->lat);
//     pos->lon = nmea_ndeg2radian(info->lon);
// }

// /**
//  * \brief Convert radians position to INFOs position
//  */
// void nmea_pos2info(const struct nmeaPOS *pos, struct nmeaINFO *info)
// {
//     info->lat = nmea_radian2ndeg(pos->lat);
//     info->lon = nmea_radian2ndeg(pos->lon);
// }

// void nmea_zero_INFO(struct nmeaINFO *info)
// {
//     memset(info, 0, sizeof(struct nmeaINFO));
//     nmea_time_now(&info->utc);
//     info->sig = NMEA_SIG_BAD;
//     info->fix = NMEA_FIX_BAD;
// }

// #ifdef _WIN32

// #include <Windows.h>

// void nmea_time_now(struct nmeaTIME *stm)
// {
//     SYSTEMTIME st;

//     GetSystemTime(&st);

//     stm->year = st.wYear - 1900;
//     stm->mon = st.wMonth - 1;
//     stm->day = st.wDay;
//     stm->hour = st.wHour;
//     stm->min = st.wMinute;
//     stm->sec = st.wSecond;
//     stm->msec = st.wMilliseconds;
// }

// #else

// #include <time.h>

// void nmea_time_now(struct nmeaTIME *stm)
// {
//     time_t lt;
//     struct tm *tt;

//     time(&lt);
//     tt = gmtime(&lt);

//     stm->year = tt->tm_year;
//     stm->mon = tt->tm_mon;
//     stm->day = tt->tm_mday;
//     stm->hour = tt->tm_hour;
//     stm->min = tt->tm_min;
//     stm->sec = tt->tm_sec;
//     stm->msec = 0;
// }

// #endif