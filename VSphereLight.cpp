#include "VSphereLight.h"
#include "util.h"

void VSphereLight::write(ostream& os) {
	os << "SphereLight {\n";
	os << "\tName \"" << Name << "\"\n";
	//os << "\tShader 1 string \"" << Shader << "\"\n";
	os << "\tShader \"" << Shader << "\"\n";
	os << "\tP " << P[0] << " " << P[1] << " " << P[2] << "\n";
	os << "\tRadius " << Radius << "\n";
	//os << "\tTransform ";
	//writeTransform(Matrix, os);
	os << "}\n\n";


}