/*
 * SkeletonNode.h
 *
 *  Created on: 2012-10-16
 *      Author: david
 */

#ifndef SKELETONNODE_H_
#define SKELETONNODE_H_

#include "ParseException.h"

#include <string>
#include <vector>
#include <fstream>

class SkeletonNode {

private:
	std::string name;
	std::vector<SkeletonNode> children;
	float offset[3];

public:
	SkeletonNode(std::ifstream& descr) throw(ParseException);
	SkeletonNode(float [3]);
	virtual ~SkeletonNode();

	void printNames(unsigned level);
};

#endif /* SKELETONNODE_H_ */
