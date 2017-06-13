#include "VShaderStd.h"

VShaderStd::VShaderStd() : DiffuseStrength(0),
							DiffuseColour(0),
							DiffuseRoughness(0),
							Spec1Strength(0),
							Spec1Colour(0),
							Spec1Roughness(0){

}

VShaderStd::~VShaderStd(){
	delete DiffuseColour;
	delete DiffuseRoughness;
	delete DiffuseStrength;
	delete Spec1Colour;
	delete Spec1Roughness;
	delete Spec1Strength;
}

void VShaderStd::write(ostream& os){
	os << "ShaderStd {\n";
	os << "\tName \"" << Name << "\"\n";
	if (DiffuseColour != 0) {
		os << "\tDiffuseColour " << DiffuseColour << "\n";
	}

	if (DiffuseRoughness != 0) {
		os << "\tDiffuseRoughness " << DiffuseRoughness << "\n";
	}

	if (DiffuseStrength != 0) {
		os << "\tDiffuseStrength " << DiffuseStrength << "\n";
	}

	if (Spec1Colour != 0) {
		os << "\tSpec1Colour " << Spec1Colour << "\n";
	}

	if (Spec1Strength != 0) {
		os << "\tSpec1Strength " << Spec1Strength << "\n";
	}

	if (Spec1Roughness != 0) {
		os << "\tSpec1Roughness " << Spec1Roughness << "\n";
	}

	os << "\tSpec1FresnelModel \"" << Spec1FresnelModel << "\"\n";

	if (Spec1FresnelModel == MString("Metal")) {
		os << "\tSpec1FresnelEdge " << Spec1FresnelEdge << "\n";
		os << "\tSpec1FresnelRefl " << Spec1FresnelRefl << "\n";
	}

	if (EmissionColour != 0) {
		os << "\tEmissionColour " << EmissionColour << "\n";
		os << "\tEmissionStrength " << EmissionStrength << "\n";
	}

	if (Transmissive) {
		os << "\tTransmissive " << 1 << "\n";
		os << "\tIOR float " << IOR << "\n";
		os << "\tPriority " << Priority << "\n";

		if (TransColour != 0) {
			os << "\tTransColour " << *TransColour << "\n";
			os << "\tTransStrength " << TransStrength << "\n";
		}

		if(Thin) {
			os << "\tThin " << 1 << "\n";

		}
	}

	os << "}\n\n";

}
