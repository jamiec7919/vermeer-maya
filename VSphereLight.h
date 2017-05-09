// Copyright 2017 The Vermeer Light Tools Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef __VSPHERELIGHT_H
#define __VSPHERELIGHT_H

#include <maya/MFStream.h>
#include <maya/MString.h>
#include <maya/MMatrix.h>

class MObject;

class VSphereLight {
public:
	void write(ostream& os);

	MString Name;
	MMatrix Matrix;
	MString Shader;
	float P[3];
	float Radius;

};

#endif // __VSphereLight_H
