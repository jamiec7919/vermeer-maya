// Copyright 2017 The Vermeer Light Tools Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef __VSTDSHADER_H
#define __VSTDSHADER_H

#include <maya/MFStream.h>
#include <maya/MString.h>

#include <maya/MColor.h>

class Uniform {
public:
	virtual void write(ostream& os) const = 0;

};

inline ostream& operator<<(ostream& out, const Uniform& r) {
		r.write(out);
	return out;
}

inline ostream& operator<<(ostream& out, const Uniform* r) {
	if (r != 0) {
		r->write(out);
	}

	return out;
}

class RGBConst : public Uniform {
public:
	float C[3];

	RGBConst() {
		C[0] = 0;
		C[1] = 0;
		C[2] = 0;

	}

	RGBConst(const MColor& col) {
		C[0] = col[0];
		C[1] = col[1];
		C[2] = col[2];

	}

	void write(ostream& os) const {
		os << "rgb " << C[0] << " " << C[1] << " " << C[2];
	}

};

class Float32Const : public Uniform {
public:
	float C;

	Float32Const() {
		C = 0;
	}

	Float32Const(float c) {
		C = c;
	}

	void write(ostream& os) const {
		os << "float " << C;
	}
};

class Texture : public Uniform {
public:
	MString Filename;

	Texture(const MString& filename) : Filename(filename) {}

	void write(ostream& os) const {
		os << "rgbtex \"" << Filename << "\"";
	}

};

class VShaderStd {
public:
	MString Name;
	Uniform* DiffuseStrength;
	Uniform* DiffuseColour;
	Uniform* DiffuseRoughness;

	Uniform* Spec1Strength;
	Uniform* Spec1Colour;
	Uniform* Spec1Roughness;
	MString Spec1FresnelModel;
	float Spec1FresnelEdge[3];
	float Spec1FresnelRefl[3];

	Uniform* EmissionStrength;
	Uniform* EmissionColour;

	Uniform* TransStrength;
	BOOL Transmissive;
	Uniform* TransColour;
	int Priority;
	float IOR;

	void write(ostream& os);

	VShaderStd();
	~VShaderStd();
};

#endif // __VSTDSHADER_H
