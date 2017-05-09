// Copyright 2017 The Vermeer Light Tools Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>

#include "VermeerStdShader.h"
#include "VShaderStd.h"

#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>

MObject  VermeerStdShader::aDiffuseStrength;
MObject  VermeerStdShader::aDiffuseColorR;
MObject  VermeerStdShader::aDiffuseColorG;
MObject  VermeerStdShader::aDiffuseColorB;
MObject  VermeerStdShader::aDiffuseColor;
MObject  VermeerStdShader::aDiffuseRoughness;

MObject  VermeerStdShader::aSpec1Strength;
MObject  VermeerStdShader::aSpec1ColorR;
MObject  VermeerStdShader::aSpec1ColorG;
MObject  VermeerStdShader::aSpec1ColorB;
MObject  VermeerStdShader::aSpec1Color;
MObject  VermeerStdShader::aSpec1Roughness;
MObject  VermeerStdShader::aSpec1FresnelModel;
MObject  VermeerStdShader::aSpec1FresnelReflR;
MObject  VermeerStdShader::aSpec1FresnelReflG;
MObject  VermeerStdShader::aSpec1FresnelReflB;
MObject  VermeerStdShader::aSpec1FresnelRefl;
MObject  VermeerStdShader::aSpec1FresnelEdgeR;
MObject  VermeerStdShader::aSpec1FresnelEdgeG;
MObject  VermeerStdShader::aSpec1FresnelEdgeB;
MObject  VermeerStdShader::aSpec1FresnelEdge;

MObject  VermeerStdShader::aTransmissive;
MObject  VermeerStdShader::aTransStrength;
MObject  VermeerStdShader::aTransColorR;
MObject  VermeerStdShader::aTransColorG;
MObject  VermeerStdShader::aTransColorB;
MObject  VermeerStdShader::aTransColor;
MObject  VermeerStdShader::aIOR;
MObject  VermeerStdShader::aPriority;

MObject  VermeerStdShader::aOutColor;
MObject  VermeerStdShader::aOutColorR;
MObject  VermeerStdShader::aOutColorG;
MObject  VermeerStdShader::aOutColorB;

// IFF type ID
// Each node requires a unique identifier which is used by
// MFnDependencyNode::create() to identify which node to create, and by
// the Maya file format.
//
// For local testing of nodes you can use any identifier between
// 0x00000000 and 0x0007ffff, but for any node that you plan to use for
// more permanent purposes, you should get a universally unique id from
// Autodesk Support. You will be assigned a unique range that you
// can manage on your own.
//
MTypeId VermeerStdShader::id(0x100);

const MString VermeerStdShader::sRegistrantId("VermeerStdShaderPlugin");
const MString VermeerStdShader::sDrawDBClassification("drawdb/shader/surface/VermeerStdShader");
const MString VermeerStdShader::sFullClassification("shader/surface:" + sDrawDBClassification);

// the postConstructor() function is called immediately after the objects
// constructor. It is not safe to call MPxNode member functions from the
// constructor, instead they should be called here.
//
void VermeerStdShader::postConstructor()
{
	// setMPSafe indicates that this shader can be used for multiprocessor
	// rendering. For a shading node to be MP safe, it cannot access any
	// shared global data and should only use attributes in the datablock
	// to get input data and store output data.
	//
	setMPSafe(true);
}

// This node does not need to perform any special actions on creation or
// destruction
//

VermeerStdShader::VermeerStdShader() { }
VermeerStdShader::~VermeerStdShader() { }

// The creator() method allows Maya to instantiate instances of this node.
// It is called every time a new instance of the node is requested by
// either the createNode command or the MFnDependencyNode::create()
// method.
//
// In this case creator simply returns a new lambert object.
//

void* VermeerStdShader::creator()
{
	return new VermeerStdShader();
}

// The initialize method is called only once when the node is first
// registered with Maya. In this method you define the attributes of the
// node, what data comes in and goes out of the node that other nodes may
// want to connect to.
//

MStatus VermeerStdShader::initialize()
{
	MFnNumericAttribute nAttr;
	MFnEnumAttribute eAttr;

	MStatus status; // Status will be used to hold the MStatus value
	// returned by each api function call. It is important
	// to check the status returned by a call to aid in
	// debugging. Failed API calls can result in subtle
	// errors that can be difficult to track down, you may
	// wish to use the CHECK_MSTATUS macro for any API
	// call where you do not need to provide your own
	// error handling.
	//

	// Attribute Initialization:
	//
	// create      - The create function creates a new attribute for the
	//				 node, it takes a long name for the attribute, a short
	//				 name for the attribute, the type of the attribute,
	//				 and a status object to determine if the api call was
	//				 successful.
	//
	// setKeyable  - Sets whether this attribute should accept keyframe
	//				 data, Attributes are not keyable by default.
	//
	// setStorable - Sets whether this attribute should be storable. If an
	//				 attribute is storable, then it will be writen out
	//				 when the node is stored to a file. Attributes are
	//               storable by default.
	//
	// setDefault  - Sets the default value for this attribute.
	//
	// setUsedAsColor - Sets whether this attribute should be presented as
	//				 a color in the UI.
	//
	// setHidden   - Sets whether this attribute should be hidden from the
	//				 UI. This is useful if the attribute is being used for
	//				 blind data, or if it is being used as scratch space
	//				 for a geometry calculation (should also be marked
	//				 non-connectable in that case). Attributes are not
	//				 hidden by default.
	//
	// setReadable - Sets whether this attribute should be readable. If an
	//				 attribute is readable, then it can be used as the
	//				 source in a dependency graph connection. Attributes
	//				 are readable by default.
	//
	// setWritable - Sets whether this attribute should be readable. If an
	//				 attribute is writable, then it can be used as the
	//				 destination in a dependency graph connection. If an
	//			     attribute is not writable then setAttr commands will
	//				 fail to change the attribute. If both keyable and
	//				 writable for an attribute are set to true it will be
	//				 displayed in the channel box when the node is
	//				 selected. Attributes are writable by default.
	//
	// setArray    - Sets whether this attribute should have an array of
	//				 data. This should be set to true if the attribute
	//				 needs to accept multiple incoming connections.
	//				 Attributes are single elements by default.
	//

	// Input Attributes
	//
	aDiffuseStrength = nAttr.create("diffuseStrength", "ds",
		MFnNumericData::kFloat, 0, &status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setKeyable(true));
	CHECK_MSTATUS(nAttr.setStorable(true));
	CHECK_MSTATUS(nAttr.setDefault(0.8f));

	aDiffuseColorR = nAttr.create("diffuseColorR", "dcr", MFnNumericData::kFloat, 0,
		&status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setKeyable(true));
	CHECK_MSTATUS(nAttr.setStorable(true));
	CHECK_MSTATUS(nAttr.setDefault(0.0f));

	aDiffuseColorG = nAttr.create("diffuseColorG", "dcg", MFnNumericData::kFloat, 0,
		&status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setKeyable(true));
	CHECK_MSTATUS(nAttr.setStorable(true));
	CHECK_MSTATUS(nAttr.setDefault(0.58824f));

	aDiffuseColorB = nAttr.create("diffuseColorB", "dcb", MFnNumericData::kFloat, 0,
		&status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setKeyable(true));
	CHECK_MSTATUS(nAttr.setStorable(true));
	CHECK_MSTATUS(nAttr.setDefault(0.644f));

	aDiffuseColor = nAttr.create("diffuseColor", "dc", aDiffuseColorR, aDiffuseColorG, aDiffuseColorB,
		&status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setKeyable(true));
	CHECK_MSTATUS(nAttr.setStorable(true));
	CHECK_MSTATUS(nAttr.setDefault(0.0f, 0.58824f, 0.644f));
	CHECK_MSTATUS(nAttr.setUsedAsColor(true));

	aDiffuseRoughness = nAttr.create("diffuseRoughness", "dr", MFnNumericData::kFloat, 0,
		&status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setKeyable(true));
	CHECK_MSTATUS(nAttr.setStorable(true));
	CHECK_MSTATUS(nAttr.setDefault(0.5f));

	aSpec1Strength = nAttr.create("spec1Strength", "s1s",
		MFnNumericData::kFloat, 0, &status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setKeyable(true));
	CHECK_MSTATUS(nAttr.setStorable(true));
	CHECK_MSTATUS(nAttr.setDefault(0.8f));

	aSpec1ColorR = nAttr.create("spec1ColorR", "s1cr", MFnNumericData::kFloat, 0,
		&status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setKeyable(true));
	CHECK_MSTATUS(nAttr.setStorable(true));
	CHECK_MSTATUS(nAttr.setDefault(0.8f));

	aSpec1ColorG = nAttr.create("spec1ColorG", "s1cg", MFnNumericData::kFloat, 0,
		&status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setKeyable(true));
	CHECK_MSTATUS(nAttr.setStorable(true));
	CHECK_MSTATUS(nAttr.setDefault(0.8f));

	aSpec1ColorB = nAttr.create("spec1ColorB", "s1cb", MFnNumericData::kFloat, 0,
		&status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setKeyable(true));
	CHECK_MSTATUS(nAttr.setStorable(true));
	CHECK_MSTATUS(nAttr.setDefault(0.8f));

	aSpec1Color = nAttr.create("spec1Color", "s1c", aSpec1ColorR, aSpec1ColorG, aSpec1ColorB,
		&status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setKeyable(true));
	CHECK_MSTATUS(nAttr.setStorable(true));
	CHECK_MSTATUS(nAttr.setDefault(0.8f, 0.8f, 0.8f));
	CHECK_MSTATUS(nAttr.setUsedAsColor(true));

	aSpec1Roughness = nAttr.create("spec1Roughness", "s1r", MFnNumericData::kFloat, 0,
		&status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setKeyable(true));
	CHECK_MSTATUS(nAttr.setStorable(true));
	CHECK_MSTATUS(nAttr.setDefault(0.5f));

	aSpec1FresnelModel = eAttr.create("spec1FresnelModel", "spec1fresmodel", 0, &status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(eAttr.addField("Dielectric", 0));
	CHECK_MSTATUS(eAttr.addField("Metal", 1));
	CHECK_MSTATUS(eAttr.setStorable(true));
	CHECK_MSTATUS(eAttr.setKeyable(false));

	aTransmissive = nAttr.create("transmissive", "t", MFnNumericData::kBoolean, 0,
		&status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setKeyable(true));
	CHECK_MSTATUS(nAttr.setStorable(true));
	CHECK_MSTATUS(nAttr.setDefault(false));

	aTransColorR = nAttr.create("transColorR", "tcr", MFnNumericData::kFloat, 0,
		&status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setKeyable(true));
	CHECK_MSTATUS(nAttr.setStorable(true));
	CHECK_MSTATUS(nAttr.setDefault(0.8f));

	aTransColorG = nAttr.create("transColorG", "tcg", MFnNumericData::kFloat, 0,
		&status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setKeyable(true));
	CHECK_MSTATUS(nAttr.setStorable(true));
	CHECK_MSTATUS(nAttr.setDefault(0.8));

	aTransColorB = nAttr.create("transColorB", "tcb", MFnNumericData::kFloat, 0,
		&status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setKeyable(true));
	CHECK_MSTATUS(nAttr.setStorable(true));
	CHECK_MSTATUS(nAttr.setDefault(0.8));

	aTransColor = nAttr.create("transColor", "tc", aTransColorR, aTransColorG, aTransColorB,
		&status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setKeyable(true));
	CHECK_MSTATUS(nAttr.setStorable(true));
	CHECK_MSTATUS(nAttr.setDefault(0.9f, 0.9f, 0.9f));
	CHECK_MSTATUS(nAttr.setUsedAsColor(true));

	aTransStrength = nAttr.create("transStrength", "ts", MFnNumericData::kFloat, 0,
		&status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setKeyable(true));
	CHECK_MSTATUS(nAttr.setStorable(true));
	CHECK_MSTATUS(nAttr.setDefault(0.8f));

	aIOR = nAttr.create("indexOfRefraction", "ior", MFnNumericData::kFloat, 0,
		&status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setKeyable(true));
	CHECK_MSTATUS(nAttr.setStorable(true));
	CHECK_MSTATUS(nAttr.setDefault(1.57f));

	aPriority = nAttr.create("priority", "p", MFnNumericData::kInt, 0,
		&status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setKeyable(true));
	CHECK_MSTATUS(nAttr.setStorable(true));
	CHECK_MSTATUS(nAttr.setDefault(0));

	// Output Attributes
	//

	// Color Output, we don't really use this but maya moans.
	//
	aOutColorR = nAttr.create("outColorR", "ocr", MFnNumericData::kFloat,
		0, &status);
	CHECK_MSTATUS(status);

	aOutColorG = nAttr.create("outColorG", "ocg", MFnNumericData::kFloat,
		0, &status);
	CHECK_MSTATUS(status);

	aOutColorB = nAttr.create("outColorB", "ocb", MFnNumericData::kFloat,
		0, &status);
	CHECK_MSTATUS(status);

	aOutColor = nAttr.create("outColor", "oc", aOutColorR, aOutColorG,
		aOutColorB, &status);
	CHECK_MSTATUS(status);

	CHECK_MSTATUS(nAttr.setHidden(false));
	CHECK_MSTATUS(nAttr.setReadable(true));
	CHECK_MSTATUS(nAttr.setWritable(false));

	// Next we will add the attributes we have defined to the node
	//
	CHECK_MSTATUS(addAttribute(aDiffuseStrength));
	CHECK_MSTATUS(addAttribute(aDiffuseColor));
	CHECK_MSTATUS(addAttribute(aDiffuseRoughness));
	CHECK_MSTATUS(addAttribute(aSpec1Strength));
	CHECK_MSTATUS(addAttribute(aSpec1Color));
	CHECK_MSTATUS(addAttribute(aSpec1Roughness));
	CHECK_MSTATUS(addAttribute(aSpec1FresnelModel));
	CHECK_MSTATUS(addAttribute(aTransmissive));
	CHECK_MSTATUS(addAttribute(aTransStrength));
	CHECK_MSTATUS(addAttribute(aTransColor));
	CHECK_MSTATUS(addAttribute(aIOR));
	CHECK_MSTATUS(addAttribute(aPriority));
	CHECK_MSTATUS(addAttribute(aOutColor));

	return(MS::kSuccess);

}

// The compute() method does the actual work of the node using the inputs
// of the node to generate its output.
//
// Compute takes two parameters: plug and data.
// - Plug is the the data value that needs to be recomputed
// - Data provides handles to all of the nodes attributes, only these
//   handles should be used when performing computations.
//
MStatus VermeerStdShader::compute(const MPlug& plug, MDataBlock& block)
{
	return(MS::kSuccess);
}

BOOL hasTexture(MPlug& plug, MString& filename) {
	MPlugArray plugs;

	plug.connectedTo(plugs, true, false);

	// see if any file textures are present
	for (int i = 0; i != plugs.length(); ++i)
	{
		// if file texture found
		if (plugs[i].node().apiType() == MFn::kFileTexture)
		{
			// bind a function set to it ....
			MFnDependencyNode fnDep(plugs[i].node());

			// will hold the txture node name
			//MString texname;
			// to get the node name
			//texname = fnDep.name();

			// get the attribute for the full texture path
			MPlug ftn = fnDep.findPlug("ftn");

			// get the filename from the attribute
			ftn.getValue(filename);

			// stop looping
			return TRUE;

		}

	}

	return FALSE;
}

Uniform* getRGB(MFnDependencyNode& nodeFn, const MString& baseShortname) {
	MPlug plug;
	RGBConst* rgbConst;

	rgbConst = new RGBConst();

	plug = nodeFn.findPlug(baseShortname + "r");

	plug.getValue(rgbConst->C[0]);

	plug = nodeFn.findPlug(baseShortname + "g");

	plug.getValue(rgbConst->C[1]);

	plug = nodeFn.findPlug(baseShortname + "b");

	plug.getValue(rgbConst->C[2]);

	plug = nodeFn.findPlug(baseShortname);

	MString filename;

	if (hasTexture(plug, filename)) {
		delete rgbConst;
		return new Texture(filename);

	}
	else {
		return rgbConst;
	}

}

Uniform* getValue(MFnDependencyNode& nodeFn, const MString& baseShortname) {
	MPlug plug;

	Float32Const* float32Const = new Float32Const();

	plug = nodeFn.findPlug(baseShortname);

	plug.getValue(float32Const->C);

	MString filename;

	if (hasTexture(plug, filename)) {
		delete float32Const;
		return new Texture(filename);

	}
	else {
		return float32Const;
	}

}

VShaderStd* VermeerStdShader::createShaderStd(MObject& obj) {
	MPlug plug;

	MFnDependencyNode nodeFn(obj);

	VShaderStd* shader = new VShaderStd();

	shader->DiffuseColour = getRGB(nodeFn, MString("dc"));
	shader->DiffuseRoughness = getValue(nodeFn, MString("dr"));
	shader->DiffuseStrength = getValue(nodeFn, MString("ds"));

	shader->Spec1Colour = getRGB(nodeFn, MString("s1c"));
	shader->Spec1Roughness = getValue(nodeFn, MString("s1r"));
	shader->Spec1Strength = getValue(nodeFn, MString("s1s"));

	shader->TransColour = getRGB(nodeFn, MString("tc"));
	shader->TransStrength = getValue(nodeFn, MString("ts"));

	plug = nodeFn.findPlug("ior");

	plug.getValue(shader->IOR);

	plug = nodeFn.findPlug("priority");

	plug.getValue(shader->Priority);

	plug = nodeFn.findPlug("transmissive");

	plug.getValue(shader->Transmissive);

	shader->Name = nodeFn.name();

	return shader;
}
