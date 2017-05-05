// Copyright 2017 The Vermeer Light Tools Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <maya/MGlobal.h>

#include <maya/MItDag.h>
#include <maya/MDagPath.h>

#include "VNFExporter.h"

#include "VCommon.h"
#include "VCamera.h"
#include "VShaderStd.h"

// Static function for Maya to allocate an instance of this object
void* VNFExporter::creator() {
	return new VNFExporter(); 
}

// The actual writer method. Note that Maya only creates the plug-in object once so anything allocated in
// members must be cleared by the end of the writer method if not needed.
MStatus	VNFExporter::writer(const MFileObject& file,
	const MString& optionsString,
	MPxFileTranslator::FileAccessMode mode) {

#if defined (OSMac_)
	char nameBuffer[MAXPATHLEN];
	strcpy(nameBuffer, file.fullName().asChar());
	const MString filename(nameBuffer);
#else
	const MString filename = file.fullName();
#endif

	ofstream newFile(filename.asChar(), ios::out);

	if (!newFile) {
		MGlobal::displayError(filename + ": could not be opened for writing");
		return MS::kFailure;
	}

	if (mode == kExportActiveAccessMode) {
		// export selected objects only

	}
	else {
		// export all objects
		globals = new VGlobals();

		globals->Camera = getActiveViewCamera();

		globals->write(newFile);

		getCameras();

		for (std::vector<VShaderStd*>::iterator iter = shaders.begin(); iter != shaders.end(); ++iter) {
			(*iter)->write(newFile);
		}

		for (std::vector<VCamera*>::iterator iter = cameras.begin(); iter != cameras.end(); ++iter) {
			(*iter)->write(newFile);
		}

	}

	clear();

	return MS::kSuccess;

}

bool VNFExporter::haveWriteMethod() const {
	return true;
}

bool VNFExporter::haveReadMethod() const {
	return false;
}

bool VNFExporter::canBeOpened() const {
	return true;

}

MString	VNFExporter::defaultExtension() const {
	return MString("vnf");
}

void VNFExporter::clear() {
	delete globals;
	globals = 0;

	for (std::vector<VShaderStd*>::iterator iter = shaders.begin(); iter != shaders.end(); ++iter) {
		delete (*iter);
	}

	shaders.clear();

	for (std::vector<VCamera*>::iterator iter = cameras.begin(); iter != cameras.end(); ++iter) {
		delete (*iter);
	}

	cameras.clear();

}

void VNFExporter::getCameras() {
	MStatus status;
	MItDag dagIter(MItDag::kBreadthFirst, MFn::kCamera, &status);

	for (; !dagIter.isDone(); dagIter.next())
	{
		MDagPath path;
		//MFnDagNode node;

		status = dagIter.getPath(path);

		if (status)
		{
			VCamera* cam = createCamera(dagIter.item());

			if (cam != 0) {
				cameras.push_back(cam);
			}
		}

		dagIter.next();
	}
}
