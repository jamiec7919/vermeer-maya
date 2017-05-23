// Copyright 2017 The Vermeer Light Tools Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <maya/MGlobal.h>

#include <maya/MItDag.h>
#include <maya/MDagPath.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MNodeClass.h>

#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MFnSet.h>
#include <maya/MFnNumericAttribute.h>

#include <maya/MFnTransform.h>

#include <maya/MFnMesh.h>
#include <maya/MPointArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFloatArray.h>
#include <maya/MItMeshPolygon.h>

#include <maya/MFnPointLight.h>

#include "VNFExporter.h"

#include "VCommon.h"
#include "VCamera.h"
#include "VSphereLight.h"
#include "VShaderStd.h"
#include "VermeerStdShader.h"
#include "util.h"

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

		VAiryFilter* filter = new VAiryFilter();
		VOutputHDR* outputHDR = new VOutputHDR(filename);

		filter->write(newFile);
		outputHDR->write(newFile);

		delete filter;
		delete outputHDR;

		exportAll(newFile);


	}

	newFile.flush();
	newFile.close();

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

	nLights = 0;
}

bool VNFExporter::hasShader(const MString& name) const{
	for (std::vector<VShaderStd*>::const_iterator iter = shaders.begin(); iter != shaders.end(); ++iter) {
		if ((*iter)->Name == name) {
			return true;
		}
	}
	return false;
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

	}
}

void VNFExporter::exportAll(ostream& ofile) {
	MStatus status;
	MItDag dagIter(MItDag::kDepthFirst);

	for (; !dagIter.isDone(); dagIter.next())
	{
		MDagPath path;
		//MFnDagNode node(dagIter.item());

		status = dagIter.getPath(path);

		if (status)
		{
			if (path.apiType() == MFn::kCamera) {
				VCamera* cam = createCamera(dagIter.currentItem());
				// MGlobal::displayInfo(path.partialPathName() + " camera.");

				if (cam != 0) {
					cameras.push_back(cam);
				}
			}
			else if (path.apiType() == MFn::kMesh) {
				// MGlobal::displayInfo(path.partialPathName() + " mesh.");
				exportMesh(ofile, dagIter.currentItem());
			}
			else if (path.apiType() == MFn::kPointLight) {
				exportPointLight(ofile, dagIter.currentItem());
			}
		}

	}

	for (std::vector<VShaderStd*>::iterator iter = shaders.begin(); iter != shaders.end(); ++iter) {
		(*iter)->write(ofile);
	}

	ofile << "\n\n# " << cameras.size() << " cameras\n\n";

	MGlobal::displayInfo(MString()+(int)(cameras.size()) + MString("cameras."));

	for (std::vector<VCamera*>::iterator iter = cameras.begin(); iter != cameras.end(); ++iter) {
		(*iter)->write(ofile);
	}

}

void VNFExporter::exportMesh(ostream& ofile, MObject& obj){
	// attach the function set to the object
	MFnMesh fn(obj);
	MFnDependencyNode nodeFn(obj);

	// this will hold the returned vertex positions
	MPointArray vts;

	// use the function set to get the points
	fn.getPoints(vts);

	ofile << "PolyMesh {\n";

	ofile << "\tName \"" << nodeFn.name() << "\"\n";

	ofile << "\tVerts 1 " << vts.length() << " point";

	for (unsigned i = 0; i < vts.length(); i++){
		ofile << " " << vts[i].x << " " << vts[i].y << " " << vts[i].z;
	}

	ofile << "\n";

	MFloatVectorArray nmls;

	// use the function set to get the points
	fn.getNormals(nmls);

	ofile << "\tNormals 1 " << nmls.length() << " vec3";

	for (unsigned i = 0; i < nmls.length(); i++){
		ofile << " " << nmls[i].x << " " << nmls[i].y << " " << nmls[i].z;
	}

	ofile << "\n";

	MStringArray uvsets;

	// get the names of the uv sets on the mesh
	fn.getUVSetNames(uvsets);

	bool haveUVs = true;

	// This is an annoying hack. if the mesh has no uv
	// coords, it always seems to return a uv set. We
	// therefore want to check for this so it causes us
	// less hassle when writing the indices later
	if (uvsets.length() == 0 || fn.numUVs(uvsets[0]) == 0) {
		haveUVs = false;

		MGlobal::displayInfo(nodeFn.name() + " has no UVs");
	}

	if (haveUVs){
		ofile << "# " << uvsets.length() << " UVs\n";

		// two arrays for the uv texturecoords
		MFloatArray u_coords;
		MFloatArray v_coords;

		// get the uv data
		fn.getUVs(u_coords, v_coords, &uvsets[0]);

		ofile << "\tUV 1 " << fn.numUVs(uvsets[0]) << " vec2";

		for (int i = 0; i < fn.numUVs(uvsets[0]); i++){
			ofile << " " << u_coords[i] << " " << v_coords[i];
		}

		ofile << "\n";

	}

	// attach an iterator to the mesh
	MItMeshPolygon itPoly(obj);

	int nFaceIdx = 0;

	ofile << "\tPolyCount " << fn.numPolygons() << " int";

	while (!itPoly.isDone()) {

		int vc = itPoly.polygonVertexCount();

		ofile << " " << vc;

		nFaceIdx += vc;

		itPoly.next();
	}

	ofile << "\n";


	itPoly.reset();

	ofile << "\tFaceIdx " << nFaceIdx << " int";

	while (!itPoly.isDone()) {

		int vc = itPoly.polygonVertexCount();

		for (int i = 0; i < vc; i++) {
			ofile << " " << itPoly.vertexIndex(i);
		}

		itPoly.next();
	}
	ofile << "\n";

	itPoly.reset();

	ofile << "\tNormalIdx " << nFaceIdx << " int";

	int nNormalIdx = 0;

	while (!itPoly.isDone()) {

		int vc = itPoly.polygonVertexCount();

		for (int i = 0; i < vc; i++) {
			ofile << " " << itPoly.normalIndex(i);
			nNormalIdx++;
		}

		itPoly.next();
	}

	ofile << "\n";

	if (nNormalIdx != nFaceIdx){
		cerr << "ERR: normalidx " << nNormalIdx << "!=" << nFaceIdx;
	}

	if (haveUVs) {
		itPoly.reset();

		ofile << "\tUVIdx " << nFaceIdx << " int";

		int nUVIdx = 0;

		while (!itPoly.isDone()) {

			int vc = itPoly.polygonVertexCount();

			int uvindex;

			for (int i = 0; i < vc; i++) {

				itPoly.getUVIndex(i, uvindex, &uvsets[0]);

				ofile << " " << uvindex;
				nUVIdx++;
			}
			itPoly.next();
		}

		ofile << "\n";

		if (nUVIdx != nFaceIdx){
			MGlobal::displayInfo(MString("ERR: uvidx ") + nUVIdx + MString(" != ") + nFaceIdx);

		}


	}

	std::vector<MString> shaders;

	MObjectArray sets;
	MObjectArray comps;
	int instanceNumber = 0;

	fn.getConnectedSetsAndMembers(instanceNumber, sets,
		comps, true);

	// iterate over all the connected sets and look for materials
	for (unsigned i = 0; i < sets.length(); i++)
	{
		MObject set = sets[i];
		MObject comp = comps[i];

		MFnSet fnset(set);
		MFnDependencyNode dnset(set);

		MObject ssattr = dnset.attribute(MString("surfaceShader"));

		MPlug ssplug(set, ssattr);

		MPlugArray srcplugarray;
		ssplug.connectedTo(srcplugarray, true, false);

		if (srcplugarray.length() == 0) continue;

		// this object contains a reference to a shader, or
		// material, so we might call our own function to write
		// that material to our own data structure for later export,
		// and return a material index so we can attach that index
		// to all the polygons we are going to extract below.
		MObject srcnode = srcplugarray[0].node();
		MFnDependencyNode nodeSrcFn(srcnode);
		shaders.push_back(nodeSrcFn.name());

		if (!hasShader(nodeSrcFn.name())) {
			createShader(srcnode);
		}

		// we might want to stop processing this node right now if
		// it is a material our exporter does not support
		//if ( material_is_not_supported( matidx )) continue;

		// otherwise, let's iterate over all the polygons that
		// are colored with this material
		//mitmeshpolygon piter( dagpath, comp );
		//for ( ; !piter.isdone(); piter.next())
		//{
		//mintarray vertexidxs;
		//piter.getvertices( vertexidxs );

		// jump to our own code to export a polygon with a material
		//writepolygon( vertexidxs, matidx );
		//}
	}

	ofile << "\tShader " << shaders.size() << " string";

	for (std::vector<MString>::iterator iter = shaders.begin(); iter != shaders.end(); ++iter) {
		ofile << " \"" << (*iter) << "\"";
	}

	ofile << "\n";

	// get the number of instances
	int NumInstances = fn.parentCount();

	MIntArray    FaceIndices;
	// this will hold references to the shaders used on the meshes
	MObjectArray Shaders;
	// get the shaders used by the i'th mesh instance
	fn.getConnectedShaders(0, Shaders, FaceIndices);
	switch (Shaders.length()) {

		// if no shader applied to the mesh instance
	case 0:
	{
		//cout << "\t\tmaterials 0\n";
	}
	break;

	// if all faces use the same material, don't export shader indices
	case 1:
	{
	}
	break;

	// if more than one material is used, write out the face indices the materials
	// are applied to.
	default:
	{
		ofile << "\tShaderIdx " << FaceIndices.length() << " int";

		for (unsigned j = 0; j < FaceIndices.length(); j++){
			ofile << " " << FaceIndices[j];
		}

		ofile << "\n";
	}
	}

	// attach a function set to this instances parent transform
	MFnTransform fnTrn(fn.parent(0));

	MMatrix matrix = fnTrn.transformationMatrix();

	MDagPath pathToMesh;
	MDagPath::getAPathTo(obj, pathToMesh);

matrix = pathToMesh.inclusiveMatrix();

	ofile << "\tTransform ";
	writeTransform(matrix,ofile);

	ofile << "}\n\n";

}

void VNFExporter::exportPointLight(ostream& ofile, MObject& obj){
	MStatus status;
	MFnPointLight fnLight;
	MFnDagNode fnDN;

	MDagPath pathToLight;
	MDagPath::getAPathTo(obj, pathToLight);
	MFnDagNode fnDagLight(pathToLight);

	fnLight.setObject(pathToLight);

	CHECK_MSTATUS( fnDN.setObject(fnDagLight.parent(0)));

	float fX, fY, fZ, radius = 0;

	MPlug plug = fnDN.findPlug("translateX",&status);

	if (MStatus::kSuccess == status)
	{
		plug.getValue(fX);
	}

	plug = fnDN.findPlug("translateY", &status);

	if (MStatus::kSuccess == status)
	{
		plug.getValue(fY);
	}

	plug = fnDN.findPlug("translateZ", &status);

	if (MStatus::kSuccess == status)
	{
		plug.getValue(fZ);
	}

	plug = fnLight.findPlug("radius", &status);

	if (MStatus::kSuccess == status)
	{
		plug.getValue(radius);
	}

	VSphereLight* l = new VSphereLight();

	l->Radius = radius;
	l->P[0] = fX;
	l->P[1] = fY;
	l->P[2] = fZ;

	MString shadername("lightshader");
	shadername += nLights;
	nLights++;

	l->Shader = shadername;
	l->Name = fnDN.name();

	l->write(ofile);

	VShaderStd* shader = new VShaderStd();

	RGBConst* rgb = new RGBConst();
	Float32Const* intensity = new Float32Const(); 

	plug = fnLight.findPlug("colorR", &status);
	if (MStatus::kSuccess == status)
	{
		plug.getValue(rgb->C[0]);
	}

	plug = fnLight.findPlug("colorG", &status);
	if (MStatus::kSuccess == status)
	{
		plug.getValue(rgb->C[1]);
	}

	plug = fnLight.findPlug("colorB", &status);
	if (MStatus::kSuccess == status)
	{
		plug.getValue(rgb->C[2]);
	}

	plug = fnLight.findPlug("intensity", &status);
	if (MStatus::kSuccess == status)
	{
		plug.getValue(intensity->C);
	}

	 intensity->C *= 10;

	 Float32Const* const1 = new Float32Const();
	 const1->C = 1;

	shader->DiffuseColour = rgb;
	shader->DiffuseStrength = const1;
	shader->EmissionColour = rgb;
	shader->EmissionStrength = intensity;

	Float32Const* spec1strength = new Float32Const();
	spec1strength->C = 0;

	shader->Spec1Strength = spec1strength;

	shader->Name = shadername;

	shaders.push_back(shader);
}

void VNFExporter::createShader(MObject& obj){
	MFnDependencyNode nodeFn(obj);
	VShaderStd* shader = 0;

	if (nodeFn.typeId() == VermeerStdShader::id)
	{
		shader = VermeerStdShader::createShaderStd(obj);
	}

	if (shader != 0) {
		shaders.push_back(shader);
	}
}

MStatus initPointLightExtensions(MObject& obj) {
	MStatus status;

	MFnNumericAttribute nAttr;
	MObject attr = nAttr.create("radius", "rad",
		MFnNumericData::kFloat, 0, &status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setKeyable(true));
	CHECK_MSTATUS(nAttr.setStorable(true));
	CHECK_MSTATUS(nAttr.setDefault(1.f));

	//MFnTypedAttribute fnAttr;
	//MObject attr = fnAttr.create(L, S, MyMPx::id); // or: MObject attr = fnAttr.create(L, S, MyMPx::id, MObject::kNullObj, &status);
	MNodeClass mnDagNodeClass("pointLight");
	status = mnDagNodeClass.addExtensionAttribute(attr);

	return status;
}
