// Copyright 2017 The Vermeer Light Tools Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef __VCOMMON_H
#define __VCOMMON_H

#include <maya/MFStream.h>
#include <maya/MString.h>

class VGlobals {
public:
	void write(ostream& os);

	VGlobals();

	int XRes, YRes;
	int MaxGoRoutines;
	MString Camera;
};

class VAiryFilter {
public:
	void write(ostream& os);

	VAiryFilter() : Name("filter1"), Res(64), Width(6) {}

	MString Name;
	int Res;
	int Width;
};

class VOutputHDR {
public:
	void write(ostream& os);

	VOutputHDR(MString fname);

	MString Filename;
};

class VOutputPNG {
public:
	void write(ostream& os);

	VOutputPNG(MString fname);

	MString Filename;
};

#endif // __VCOMMON_H
