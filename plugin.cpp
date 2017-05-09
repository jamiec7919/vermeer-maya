// Copyright 2017 The Vermeer Light Tools Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <maya/MStatus.h>
#include <maya/MObject.h>
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>

#include "VNFExporter.h"
#include "VermeerStdShader.h"

// specifies a script for the Export Options UI
char* g_OptionScript = "VNFFileExportScript";

// the default option string
char* g_DefaultExportOptions = "";

MStatus initializePlugin(MObject obj) {
	MStatus status;
	MFnPlugin plugin(obj, "Vermeer Light Tools", "0.3.0", "Any");

	// Register the translator with the system
	//
	status = plugin.registerFileTranslator("VNF File",
		"",
		VNFExporter::creator,
		g_OptionScript,
		g_DefaultExportOptions,
		true);

	if (!status) {
		status.perror("registerFileTranslator");
		return status;
	}

	MString command("if( `window -exists createRenderNodeWindow` ) {refreshCreateRenderNodeWindow(\"");

	CHECK_MSTATUS(plugin.registerNode("VermeerStdShader", VermeerStdShader::id,
		VermeerStdShader::creator, VermeerStdShader::initialize, MPxNode::kDependNode,
		&VermeerStdShader::sFullClassification));

	/*
	CHECK_MSTATUS(
	MHWRender::MDrawRegistry::registerSurfaceShadingNodeOverrideCreator(
	sDrawDBClassification,
	sRegistrantId,
	lambertShaderOverride::creator));
	*/

	command += VermeerStdShader::sFullClassification;

	command += "\");}\n";

	CHECK_MSTATUS(MGlobal::executeCommand(command));

	CHECK_MSTATUS(initPointLightExtensions(obj));

	return MS::kSuccess;
}


MStatus uninitializePlugin(MObject obj) {
	MStatus   status;
	MFnPlugin plugin(obj);

	status = plugin.deregisterFileTranslator("VNF File");

	if (!status) {
		status.perror("deregisterFileTranslator");
		return status;
	}

	MString command("if( `window -exists createRenderNodeWindow` ) {refreshCreateRenderNodeWindow(\"");

	CHECK_MSTATUS(plugin.deregisterNode(VermeerStdShader::id));

	/*
	CHECK_MSTATUS(MHWRender::MDrawRegistry::deregisterSurfaceShadingNodeOverrideCreator(
	sDrawDBClassification, sRegistrantId));
	*/
	command += VermeerStdShader::sFullClassification;
	command += "\");}\n";

	CHECK_MSTATUS(MGlobal::executeCommand(command));

	return MS::kSuccess;
}
