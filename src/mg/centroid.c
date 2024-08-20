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

#include "mgp.h"

struct pri_centriod {
    struct mg_point p_cent_sum;
    size_t pt_num;

    struct mg_point l_cent_sum;
    double total_length;
        
    double total_area;
    double total_ax;
    double total_ay;
};

void pri_centriod_single(const struct mg_object *obj,
                         struct pri_centriod *centriod)
{
    assert(obj);
    if (obj->gdim == 0) {
        centriod->p_cent_sum.x += obj->pp[0];
        centriod->p_cent_sum.y += obj->pp[1];
        centriod->pt_num += 1;
    }
    else if (obj->gdim == 1) {
        size_t npts = obj->npoints;
        double line_len = 0.0;
        for (size_t i = 0; i < npts - 1; ++i) {
            double x1 = obj->pp[i * obj->cdim];
            double y1 = obj->pp[i * obj->cdim + 1];
            double x2 = obj->pp[(i + 1) * obj->cdim];
            double y2 = obj->pp[(i + 1) * obj->cdim + 1];
            double segment_len = MG_POINTDISTANCE(x1, y1, x2, y2);
            if (segment_len == 0.0)
                continue;

            line_len += segment_len;
            double midx = (x1 + x2) * 0.5;
            double midy = (y1 + y2) * 0.5;
            centriod->l_cent_sum.x += segment_len * midx;
            centriod->l_cent_sum.y += segment_len * midy;
        }
        centriod->total_length += line_len;
        if (line_len == 0.0 && npts > 0) {
            centriod->p_cent_sum.x += obj->pp[0];
            centriod->p_cent_sum.y += obj->pp[1];
            centriod->pt_num += 1;
        }
    }
    else if (obj->gdim == 2) {
        double area = mg_prop_area_value(obj);
        double tx = 0.0;
        double ty = 0.0;
        for(int i = 0; i < obj->npoints; ++i)
        {
            tx += obj->pp[i * obj->cdim];
            ty += obj->pp[i * obj->cdim + 1];
        }
        centriod->total_area += area;
        centriod->total_ax += (tx / obj->npoints);
        centriod->total_ay += (ty / obj->npoints);
    }
}

void mg_prop_geo_centriod(const struct mg_object *obj, double* xy)
{
    assert(obj);
    struct pri_centriod centriod;
    if (obj->ngeoms == 1) {
        pri_centriod_single(obj, &centriod);
    }
    else {
        for (int i = 0; i < obj->ngeoms; ++i) {
            pri_centriod_single(obj->objects[i], &centriod);
        }
    }

    if(obj->gdim == 0)
    {
        xy[0] = centriod.p_cent_sum.x / centriod.pt_num;
        xy[1] = centriod.p_cent_sum.y / centriod.pt_num;
    }
    else if(obj->gdim == 1)
    {
        xy[0] = centriod.l_cent_sum.x / centriod.total_length;
        xy[1] = centriod.l_cent_sum.y / centriod.total_length;  
    }
    else if(obj->gdim == 2)
    {
        xy[0] = centriod.total_area / centriod.total_ax;
        xy[1] = centriod.total_area / centriod.total_ay; 
    }
}