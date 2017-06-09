#include "VCamera.h"
#include "util.h"

#include <maya/MFnCamera.h>
#include <maya/MDagPath.h>
#include <maya/M3DView.h>

void VCamera::write(ostream& os) {
	os << "Camera {\n";
	os << "\tType \"Matrix\"\n";
	os << "\tName \"" << Name << "\"\n";
	os << "\tAspect " << Aspect << "\n";
	os << "\tFov " << FOV << "\n";
	os << "\tFocal " << Focus << "\n";
	os << "\tRadius " << Radius << "\n";
	os << "\tWorldToLocal ";
	writeTransform(Matrix, os);
	os << "}\n\n";

}

MString getActiveViewCamera() {
	MStatus status;

	// Should check if ortho and warn/not do.

	M3dView view = M3dView::active3dView(&status);

	MDagPath cam;

	status = view.getCamera(cam);

	if (status == MS::kSuccess) {
		return cam.partialPathName();

	}

	return "";
}

VCamera* createCamera(MObject& obj) {
	MFnCamera fn(obj);
	MDagPath path;

	// ignore orthographic cameras
	if (fn.isOrtho())
		return 0;

	// Should probably ignore non-renderables too.

	VCamera* cam = new VCamera;

	MDagPath::getAPathTo(obj, path);

	//MTransformationMatrix trn(path.inclusiveMatrixInverse());
	//double scale[3] = { 1, -1, 1 };
	//trn.setScale(scale,MSpace::kPreTransform);

	cam->Name = path.partialPathName();
	cam->Matrix = path.inclusiveMatrixInverse();
	// This is a hack, need to confirm the correct handedness and if images are flipped in vermeer
	cam->Matrix(0, 1) = -cam->Matrix(0, 1);
	cam->Matrix(1, 1) = -cam->Matrix(1, 1);
	cam->Matrix(2, 1) = -cam->Matrix(2, 1);
	cam->Matrix(3, 1) = -cam->Matrix(3, 1);
	cam->Radius = 0;
	cam->FOV = fn.horizontalFieldOfView() * (180 / M_PI);
	cam->Aspect = fn.aspectRatio();
	cam->Focus = fn.focusDistance();

	return cam;
}