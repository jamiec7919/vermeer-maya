// Copyright 2017 The Vermeer Light Tools Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef __UTIL_H
#define __UTIL_H

#include <maya/MFStream.h>
#include <maya/MMatrix.h>
#include <vector>

void writeTransform(const MMatrix& trn, ostream& os) {
	os << "1 matrix " << trn(0, 0) << " " << trn(1, 0) << " " << trn(2, 0) << " " << trn(3, 0) << "\n";
	os << "\t\t" << trn(0, 1) << " " << trn(1, 1) << " " << trn(2, 1) << " " << trn(3, 1) << "\n";
	os << "\t\t" << trn(0, 2) << " " << trn(1, 2) << " " << trn(2, 2) << " " << trn(3, 2) << "\n";
	os << "\t\t" << trn(0, 3) << " " << trn(1, 3) << " " << trn(2, 3) << " " << trn(3, 3) << "\n";

}

void writeTransforms(const std::vector<MMatrix>& trn, ostream& os) {
	os << trn.size() << " matrix ";
	
	for (std::vector<MMatrix>::const_iterator iter = trn.begin(); iter != trn.end(); ++iter) {
		os << "\t\t" << (*iter)(0, 0) << " " << (*iter)(1, 0) << " " << (*iter)(2, 0) << " " << (*iter)(3, 0) << "\n";
		os << "\t\t" << (*iter)(0, 1) << " " << (*iter)(1, 1) << " " << (*iter)(2, 1) << " " << (*iter)(3, 1) << "\n";
		os << "\t\t" << (*iter)(0, 2) << " " << (*iter)(1, 2) << " " << (*iter)(2, 2) << " " << (*iter)(3, 2) << "\n";
		os << "\t\t" << (*iter)(0, 3) << " " << (*iter)(1, 3) << " " << (*iter)(2, 3) << " " << (*iter)(3, 3) << "\n";
	}
}

#endif // __UTIL_H
