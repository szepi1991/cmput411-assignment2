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

struct MotionFrame {
	// these can't be const since they are put in a vector where the elements
	// need to be assignable, .. but they really should not be changed!
private:
	unsigned channels;
	float zRot, yRot, xRot;
	float xPos, yPos, zPos;
	float modelTrans[16]; // all transformations of model -- in matrix format

public:

	MotionFrame(double zRot, double yRot, double xRot,
			double xPos, double yPos, double zPos) :
				zRot(zRot), yRot(yRot), xRot(xRot),
				xPos(xPos), yPos(yPos), zPos(zPos) {
		channels = 6;
		genMatrix();
	}
	MotionFrame(double zRot, double yRot, double xRot) :
				zRot(zRot), yRot(yRot), xRot(xRot) {
		xPos = 0; yPos = 0; zPos = 0;
		channels = 3;
		genMatrix();
	}

	void genMatrix();
	void applyTransformation();
	void printMatrix();
};

class SkeletonNode {

private:
	static unsigned nodeCounter;
	unsigned myCounter;

	std::string name;
	std::vector<SkeletonNode> children;
	boost::array<float, 3> offset;
	unsigned channelNum;
	std::vector<MotionFrame> motion;

public:
	SkeletonNode(std::ifstream& descr) throw(ParseException);
	SkeletonNode(boost::array<float, 3> const &);
	virtual ~SkeletonNode();
	std::string getDescr();

	void printNames(unsigned level);
	void display(int);
	void addAnimationFrame(std::ifstream& descr);
	boost::array<float, 3> getEndPoint() throw(int);
};

#endif /* SKELETONNODE_H_ */
