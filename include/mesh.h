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

#ifndef MESH_H
#define MESH_H

#ifdef __cplusplus
extern "C" {
#endif

struct mm_texture {
    char *name;
    char *path;
};

struct mm_material {
    /* Material name */
    char *name;

    /* Parameters */
    float Ka[3]; /* Ambient */
    float Kd[3]; /* Diffuse */
    float Ks[3]; /* Specular */
    float Ke[3]; /* Emission */
    float Kt[3]; /* Transmittance */
    float Ns;    /* Shininess */
    float Ni;    /* Index of refraction */
    float Tf[3]; /* Transmission filter */
    float d;     /* Disolve (alpha) */
    int illum;   /* Illumination model */

    /* Set for materials that don't come from the associated mtllib */
    int fallback;

    /* Texture map indices in fastObjMesh textures array */
    unsigned int map_Ka;
    unsigned int map_Kd;
    unsigned int map_Ks;
    unsigned int map_Ke;
    unsigned int map_Kt;
    unsigned int map_Ns;
    unsigned int map_Ni;
    unsigned int map_d;
    unsigned int map_bump;
};

#ifdef __cplusplus
}
#endif

#endif