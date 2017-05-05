// Copyright 2017 The Vermeer Light Tools Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef __VNFEXPORTER_H
#define __VNFEXPORTER_H

#include <maya/MPxFileTranslator.h>
#include <vector>

class VShaderStd;
class VGlobals;
class VCamera;

class VNFExporter : public MPxFileTranslator {

public:
	VNFExporter() {}
	virtual					~VNFExporter(){}

	virtual MStatus			writer(const MFileObject& file,
		const MString& optionsString,
		MPxFileTranslator::FileAccessMode mode);

	virtual bool			haveWriteMethod() const;
	virtual bool			haveReadMethod() const;
	virtual	bool			canBeOpened() const;

	virtual MString			defaultExtension() const;

	static void* VNFExporter::creator();

	void clear();

	void getCameras();

	std::vector<VShaderStd*> shaders;
	std::vector<VCamera*> cameras;
	VGlobals* globals;

};

#endif /*__VNFEXPORTER_H*/
