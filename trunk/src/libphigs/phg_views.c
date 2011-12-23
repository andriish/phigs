/******************************************************************************
*   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
*
*   This file is part of Open PHIGS
*   Copyright (C) 2011 Surplus Users Ham Society
*
*   Open PHIGS is free software: you can redistribute it and/or modify
*   it under the terms of the GNU Lesser General Public License as published by
*   the Free Software Foundation, either version 2.1 of the License, or
*   (at your option) any later version.
*
*   Open PHIGS is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU Lesser General Public License for more details.
*
*   You should have received a copy of the GNU Lesser General Public License
*   along with Open PHIGS. If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <phigs/phg.h>
#include <phigs/ws.h>
#include <phigs/css.h>
#include <phigs/alloc.h>

Pview_rep3 default_views[WS_MIN_PREDEF_VIEW_REPS];

void phg_init_default_views(void)
{
    Pview_rep3	*view = default_views;
    int		i;

    phg_mat_identity( view[0].ori_matrix );
    phg_mat_identity( view[0].map_matrix );
    view[0].clip_limit.x_min = view[0].clip_limit.y_min =
	view[0].clip_limit.z_min = 0.0;
    view[0].clip_limit.x_max = view[0].clip_limit.y_max =
	view[0].clip_limit.z_max = 1.0;
    view[0].xy_clip = view[0].back_clip = view[0].front_clip = PIND_CLIP;
    for ( i = 1; i < WS_MIN_PREDEF_VIEW_REPS; i++ )
	view[i] = view[0];

    /* Predefined some interesting views. */
    /* View 1: parallel front view in lower left corner of ws window.
     * vrp = (0,0,0), vup = <0,1,0>, vpn = <0,0,1>, prp = (0.5,0.5,5.0)
     * win x limits = [0,1], win y limits = [0,1]
     * view plane = 0.0, front plane = 1.0, back plane = 0.0
     * vp x limits = [.1,.4], vp y limits = [.1,.4], vp z limits = [0,1]
     */
    view[1].map_matrix[0][0] = 0.3;
    view[1].map_matrix[0][3] = 0.1;
    view[1].map_matrix[1][1] = 0.3;
    view[1].map_matrix[1][3] = 0.1;

    /* View 2: top view in upper left corner of ws window.
     * vrp = (0,0,0), vup = <0,0,-1>, vpn = <0,1,0>, prp = (0.5,-0.5,5.0)
     * win x limits = [0,1], win y limits = [-1,0]
     * view plane = 0.0, front plane = 1.0, back plane = 0.0
     * vp x limits = [.1,.4], vp y limits = [.6,.9], vp z limits = [0,1]
     */
    view[2].map_matrix[0][0] = 0.3;
    view[2].map_matrix[0][3] = 0.1;
    view[2].map_matrix[1][1] = 0.3;
    view[2].map_matrix[1][3] = 0.9;
    view[2].ori_matrix[1][1] =  0.0;
    view[2].ori_matrix[1][2] = -1.0;
    view[2].ori_matrix[2][1] =  1.0;
    view[2].ori_matrix[2][2] =  0.0;

    /* View 3: right side view in lower right corner of ws window.
     * vrp = (0,0,0), vup = <0,1,0>, vpn = <1,0,0>, prp = (-0.5,0.5,5.0)
     * win x limits = [-1,0], win y limits = [0,1]
     * view plane = 0.0, front plane = 1.0, back plane = 0.0
     * vp x limits = [.6,.9], vp y limits = [.1,.4], vp z limits = [0,1]
     */
    view[3].map_matrix[0][0] = 0.3;
    view[3].map_matrix[0][3] = 0.9;
    view[3].map_matrix[1][1] = 0.3;
    view[3].map_matrix[1][3] = 0.1;
    view[3].ori_matrix[0][0] =  0.0;
    view[3].ori_matrix[0][2] = -1.0;
    view[3].ori_matrix[2][0] =  1.0;
    view[3].ori_matrix[2][2] =  0.0;

    /* View 4: off-axis view in upper right corner of ws window.
     * vrp = (0,0,0), vup = <0,1,0>, vpn = <1,1,1>, prp = (0,0,5)
     * win x limits = [-a,a], win y limits = [-a,a], a = 1/sqrt(2)
     * view plane = 0.0, front plane = sqrt(3), back plane = 0.0
     * vp x limits = [.6,.9], vp y limits = [.6,.9], vp z limits = [0,1]
     */
    view[4].map_matrix[0][0] = 0.3 / sqrt(2.0);
    view[4].map_matrix[1][1] = 0.3 / sqrt(2.0);
    view[4].map_matrix[2][2] = -1.0 / sqrt(3.0); /* Why negative */
    view[4].map_matrix[0][3] = 0.75;
    view[4].map_matrix[1][3] = 0.75;
    view[4].ori_matrix[0][0] =
        -(view[4].ori_matrix[0][2] = -1.0/sqrt(2.0));
    view[4].ori_matrix[2][0] =
        view[4].ori_matrix[2][1] =
        view[4].ori_matrix[2][2] =  1.0/sqrt(3.0);
    view[4].ori_matrix[1][0] =
        view[4].ori_matrix[1][2] = -1.0/(sqrt(3.0) * sqrt(2.0));
    view[4].ori_matrix[1][1] = -2.0 * view[4].ori_matrix[1][0];

    /* View 5: off-axis perspective view in whole of ws window.
     * vrp = (0,0,0), vup = <0,1,0>, vpn = <1,1,1>, prp = (0,0,20)
     * win x limits = [-a,a], win y limits = [-a,a], a = 1/sqrt(2)
     * view plane = 10.0, front plane = sqrt(3), back plane = 0.0
     * vp x limits = [0,1], vp y limits = [0,1], vp z limits = [0,1]
     */
    memcpy(view[5].ori_matrix,
           view[4].ori_matrix,
	   sizeof(Pmatrix3));
    view[5].map_matrix[0][0] =  0.5 / sqrt(2.0);
    view[5].map_matrix[1][1] =  0.5 / sqrt(2.0);
    view[5].map_matrix[0][2] = -0.025;
    view[5].map_matrix[1][2] = -0.025;
    view[5].map_matrix[2][2] = -(1.0/sqrt(3.0) - 1.0/20.0); /* Why negative */
    view[5].map_matrix[0][3] = 0.5;
    view[5].map_matrix[1][3] = 0.5;
    view[5].map_matrix[3][2] = -1.0/20.0;
}

