/*
 * tools.h
 *
 *  Created on: 2012-10-16
 *      Author: david
 */

#ifndef TOOLS_H_
#define TOOLS_H_

#include <string>
#include <cassert>
#include <algorithm>
#include <iostream>


#define DEBUG true
#define SECtoMSEC 1000


// typedef boost::shared_ptr<SkeletonNode> SkeletonNodePtr;

inline std::string strToUpper(std::string strToConvert) {
    std::transform(strToConvert.begin(), strToConvert.end(), strToConvert.begin(), ::toupper);

    return strToConvert;
}

inline void confirmParse(std::string expected, std::string got) throw(ParseException) {
	if (expected.compare(got) != 0) throw (ParseException(expected, got));
}


#endif /* TOOLS_H_ */
