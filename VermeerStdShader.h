// Copyright 2017 The Vermeer Light Tools Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef __VERMEERSTDSHADER_H
#define __VERMEERSTDSHADER_H

#include <maya/MPxNode.h>

class VShaderStd;

class VermeerStdShader : public MPxNode
{
public:
	VermeerStdShader();
	virtual			~VermeerStdShader();

	static  void *	creator();
	virtual MStatus	compute(const MPlug&, MDataBlock&);
	static  MStatus	initialize();

	static VShaderStd* createShaderStd(MObject obj);

	// postConstructor:
	// The postConstructor method allows us to call MPxNode member
	// functions during initialization. Internally maya creates two
	// objects when a user defined node is created, the internal MObject
	// and the user derived object. The association between the these
	// two objects is not made until after the MPxNode constructor is
	// called. This implies that no MPxNode member function can be called
	// from the MPxNode constructor. The postConstructor will get called
	// immediately after the constructor when it is safe to call any
	// MPxNode member function.
	//

	virtual void	postConstructor();

	static  MTypeId   id;  // The IFF type id

	static const MString VermeerStdShader::sRegistrantId;
	static const MString VermeerStdShader::sDrawDBClassification;
	static const MString VermeerStdShader::sFullClassification;

protected:

	// Diffuse weight
	static MObject  aDiffuseStrength;

	// Red component of surface color
	static MObject  aDiffuseColorR;

	// Green component of surface color
	static MObject  aDiffuseColorG;

	// Blue component of surface color
	static MObject  aDiffuseColorB;

	// Surface color
	static MObject  aDiffuseColor;

	// Surface roughness
	static MObject  aDiffuseRoughness;

	// Spec1 Weight
	static MObject  aSpec1Strength;

	// Red component of surface color
	static MObject  aSpec1ColorR;

	// Green component of surface color
	static MObject  aSpec1ColorG;

	// Blue component of surface color
	static MObject  aSpec1ColorB;

	// Surface color
	static MObject  aSpec1Color;

	// Surface roughness
	static MObject  aSpec1Roughness;

	// Surface fresnel
	static MObject  aSpec1FresnelModel;
	static MObject  aSpec1FresnelEdgeR;
	static MObject  aSpec1FresnelEdgeG;
	static MObject  aSpec1FresnelEdgeB;
	static MObject  aSpec1FresnelEdge;
	static MObject  aSpec1FresnelReflR;
	static MObject  aSpec1FresnelReflG;
	static MObject  aSpec1FresnelReflB;
	static MObject  aSpec1FresnelRefl;

	// 
	static MObject  aTransStrength;

	// Transmission
	static MObject  aTransmissive;

	// Red component of transmission color
	static MObject  aTransColorR;

	// Green component of transmission color
	static MObject  aTransColorG;

	// Blue component of transmission color
	static MObject  aTransColorB;

	// Transmission color
	static MObject  aTransColor;

	// Index of refraction
	static MObject  aIOR;

	// Refractive media priority value (int) 
	static MObject  aPriority;

	// Red component of output color
	static MObject  aOutColorR;

	// Green component of output color
	static MObject  aOutColorG;

	// Blue component of output color
	static MObject  aOutColorB;

	// Output color
	static MObject  aOutColor;


};

#endif // __VERMEERSTDSHADER_H
