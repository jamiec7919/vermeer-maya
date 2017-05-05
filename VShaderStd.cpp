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

	os << "\tDiffuseRoughness " << DiffuseRoughness << "\n";

	os << "\tDiffuseStrength " << DiffuseStrength << "\n";

	if (Spec1Colour != 0) {
		os << "\tSpec1Colour " << Spec1Colour << "\n";
	}

	os << "\tSpec1Strength " << Spec1Strength << "\n";

	os << "\tSpec1Roughness " << Spec1Roughness << "\n";

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

	}

	os << "}\n\n";

}
