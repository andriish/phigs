/******************************************************************************
*   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
*
*   This file is part of Open PHIGS
*   Copyright (C) 2011 - 2012 Surplus Users Ham Society
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

/* Mesa-GLC library version 0.3
 * Copyright (C) 2000 Paul Brannan
 * This library is released under the GNU LGPL.  For more information, please
 * see the file LICENSE, or visit the GNU web page at www.gnu.org.
 */

#ifndef _GLC_H
#define _GLC_H

#include <GL/gl.h>

typedef char GLCchar;

typedef enum {
	GLC_NONE = 0,
	GLC_AUTO_FONT			= 0x0010,
	GLC_GL_OBJECTS			= 0x0011,
	GLC_MIPMAP			= 0x0012,
	GLC_OP_glcUnmappedCode		= 0x0020,
	GLC_BASELINE			= 0x0030,
	GLC_BOUNDS			= 0x0031,
	GLC_PARAMETER_ERROR		= 0x0040,
	GLC_RESOURCE_ERROR		= 0x0041,
	GLC_STATE_ERROR			= 0x0042,
	GLC_CHAR_LIST			= 0x0050,
	GLC_FACE_LIST			= 0x0051,
	GLC_FAMILY			= 0x0060,
	GLC_MASTER_FORMAT		= 0x0061,
	GLC_VENDOR			= 0x0062,
	GLC_VERSION			= 0x0063,
	GLC_CHAR_COUNT			= 0x0070,
	GLC_FACE_COUNT			= 0x0071,
	GLC_IS_FIXED_PITCH		= 0x0072,
	GLC_MAX_MAPPED_CODE		= 0x0073,
	GLC_MIN_MAPPED_CODE		= 0x0074,
	GLC_CATALOG_LIST		= 0x0080,
	GLC_CURRENT_FONT_LIST		= 0x0090,
	GLC_FONT_LIST			= 0x0091,
	GLC_LIST_OBJECT_LIST		= 0x0092,
	GLC_TEXTURE_OBJECT_LIST		= 0x0093,
	GLC_DATA_POINTER		= 0x00A0,
	GLC_EXTENSIONS			= 0x00B0,
	GLC_RELEASE			= 0x00B1,
	GLC_RESOLUTION			= 0x00C0,
	GLC_BITMAP_MATRIX		= 0x00D0,
	GLC_CATALOG_COUNT		= 0x00E0,
	GLC_CURRENT_FONT_COUNT		= 0x00E1,
	GLC_FONT_COUNT			= 0x00E2,
	GLC_LIST_OBJECT_COUNT		= 0x00E3,
	GLC_MASTER_COUNT		= 0x00E4,
	GLC_MEASURED_CHAR_COUNT		= 0x00E5,
	GLC_RENDER_STYLE		= 0x00E6,
	GLC_REPLACEMENT_CODE		= 0x00E7,
	GLC_STRING_TYPE			= 0x00E8,
	GLC_TEXTURE_OBJECT_COUNT	= 0x00E9,
	GLC_VERSION_MAJOR		= 0x00EA,
	GLC_VERSION_MINOR		= 0x00EB,
	GLC_BITMAP			= 0x0100,
	GLC_LINE			= 0x0101,
	GLC_TEXTURE			= 0x0102,
	GLC_TRIANGLE			= 0x0103,
	GLC_UCS1			= 0x0110,
	GLC_UCS2			= 0x0111,
	GLC_UCS4			= 0x0112
} GLCenum;
	

/* Context functions */
GLint glcGenContext(void);
void glcContext(GLint context);
void glcDeleteContext(GLint context);
GLint* glcGetAllContexts(void);
GLint glcGetCurrentContext(void);
GLboolean glcIsContext(GLint context);

/* Rendering functions */
void glcRenderChar(char c);
void glcRenderString(const char *s);
void glcRenderCountedString(GLint count, const GLCchar* s);

/* Font functions */
void glcFont(GLint font);
GLboolean glcFontFace(GLint fontnum, const char *style);
GLint glcNewFontFromFamily(GLint fontnum, const char *s);
GLboolean glcIsFont(GLint font);
void glcDeleteFont(GLint font);

/* Information-retrival functions */
const GLCchar* glcGetc(GLCenum attrib);
GLfloat glcGetf(GLCenum attrib);
GLfloat* glcGetfv(GLCenum attrib, GLfloat *vec);
GLint glcGeti(GLCenum attrib);
GLboolean glcIsEnabled(GLCenum attrib);
void glcGenFontID(void);

/* Error-handling functions */
GLCenum glcGetError(void);

/* Data pointer functions */
void glcDataPointer(GLvoid *pointer);
GLvoid* glcGetPointer(GLCenum attrib);

/* Bitmap matrix functions */
void glcLoadIdentity(void);
void glcLoadMatrix(const GLfloat *matrix);
void glcMultMatrix(const GLfloat *matrix);
void glcRotate(GLfloat angle);
void glcScale(GLfloat x, GLfloat y);

#endif
