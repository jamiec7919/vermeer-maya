#include "VCommon.h"

#include <maya/MCommonRenderSettingsData.h>
#include <maya/MRenderUtil.h>

VGlobals::VGlobals() {
	MCommonRenderSettingsData rsData;
	MRenderUtil::getCommonRenderSettings(rsData);

	MaxGoRoutines = 5;
	XRes = rsData.width;
	YRes = rsData.height;
}

void VGlobals::write(ostream& os) {
	os << "Globals {\n";
	os << "\tXRes " << XRes << "\n";
	os << "\tYRes " << YRes << "\n";
	os << "\tMaxGoRoutines " << MaxGoRoutines << "\n";
	os << "\tCamera \"" << Camera << "\"\n";
	os << "}\n\n";

}

void VAiryFilter::write(ostream& os) {
	os << "AiryFilter {\n";
	os << "\tName \"" << Name << "\"\n";
	os << "\tWidth " << Width << "\n";
	os << "\tRes " << Res << "\n";
	os << "}\n\n";

}

VOutputHDR::VOutputHDR(MString fname) : Filename(fname + ".hdr") {
	
	MCommonRenderSettingsData rsData;
	MRenderUtil::getCommonRenderSettings(rsData);

	if (rsData.name != "") {
		Filename = MString(rsData.name + ".hdr");
	}
}

void VOutputHDR::write(ostream& os) {
	os << "OutputHDR {\n";
	os << "\tFilename \"" << Filename << "\"\n";
	os << "}\n\n";

}

