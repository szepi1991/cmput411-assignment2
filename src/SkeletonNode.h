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
#include <boost/array.hpp>

class SkeletonNode {

private:
	std::string name;
	std::vector<SkeletonNode> children;
	boost::array<float, 3> offset;
	unsigned myCounter;
	static unsigned nodeCounter;

public:
	SkeletonNode(std::ifstream& descr) throw(ParseException);
	SkeletonNode(boost::array<float, 3>);
	virtual ~SkeletonNode();

	void printNames(unsigned level);
	void display();
	boost::array<float, 3> getEndPoint() throw(int);
};

#endif /* SKELETONNODE_H_ */
