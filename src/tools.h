/*
 * tools.h
 *
 *  Created on: 2012-10-16
 *      Author: david
 */

#ifndef TOOLS_H_
#define TOOLS_H_

#include "ParseException.h"

#include <string>
#include <cassert>
#include <algorithm>
#include <iostream>


#define MYINFO false
#define SECtoMSEC 1000.0f // normal speed
//#define SECtoMSEC 10000.0f

#define PI 3.14159265359
#define EPS 0.0000001

#define degToRad(x) (x*PI/180)
#define radToDeg(x) (x*180/PI)


// typedef boost::shared_ptr<SkeletonNode> SkeletonNodePtr;

inline std::string strToUpper(std::string strToConvert) {
    std::transform(strToConvert.begin(), strToConvert.end(), strToConvert.begin(), ::toupper);

    return strToConvert;
}

inline void confirmParse(std::string expected, std::string got) throw(ParseException) {
	if (expected.compare(got) != 0) throw (ParseException(expected, got));
}

// prints a 4x4 matrix
inline void print4x4Matrix(float * toPrint) {
	std::cout << std::fixed;
	std::cout.precision(6);
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			std::cout << "\t" << toPrint[i*4+j];
		}
		std::cout << std::endl;
	}
}

inline bool same4x4Matrix(float * a, float * b) {
	for (unsigned i = 0; i < 16; ++i) {
		if (abs(a[i] - b[i]) > EPS) return false;
	}
	return true;
}


#endif /* TOOLS_H_ */
