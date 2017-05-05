// Copyright 2017 The Vermeer Light Tools Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef __VCAMERA_H
#define __VCAMERA_H

#include <maya/MFStream.h>
#include <maya/MString.h>
#include <maya/MMatrix.h>

class MObject;

class VCamera {
public:
	void write(ostream& os);

	MString Name;
	MMatrix Matrix;
	float Radius;
	float FOV;
	float Focus;
	float Aspect;

};

VCamera* createCamera(MObject& obj);

// Return the name of the camera for the active 3D view.s
MString getActiveViewCamera();

#endif // __VCAMERA_H
