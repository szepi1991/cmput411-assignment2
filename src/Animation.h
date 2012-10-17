/*
 * Animation.h
 *
 *  Created on: 2012-10-16
 *      Author: david
 */

#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <string>
#include <boost/shared_ptr.hpp>

#include "SkeletonNode.h"
#include "ParseException.h"

class Animation {
private:
	std::string filename;
	boost::shared_ptr<SkeletonNode> root; // TODO should be shared_ptr..
public:
	Animation(char *filename) throw(ParseException);
	virtual ~Animation();

	std::string getFileName() {return filename;}
};

#endif /* ANIMATION_H_ */
