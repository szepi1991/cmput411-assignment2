/*
 * SkeletonNode.cpp
 *
 *  Created on: 2012-10-16
 *      Author: david
 */


#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include "SkeletonNode.h"
#include "tools.h"


void confirmParse(std::string expected, std::string got) throw(ParseException) {
	if (expected.compare(got) != 0) throw (ParseException(expected, got));
}

/* Recursively creates the skeleton based on the info in descr.
 * Reads out just as much info from the stream as needed.
 *
 * descr should contain info in the format
 * BoneName
 * {
 *     OFFSET f f f
 *     CHANNELS ----
 *     JOINT <info about inner node>
 * }
 * Or sometimes:
 * BoneName
 * {
 *     OFFSET f f f
 *     CHANNELS ----
 *     End Site
 *     {
 *         OFFSET f f f
 *     }
 * }
 */
SkeletonNode::SkeletonNode(std::ifstream& descr) throw(ParseException) {
	myCounter = nodeCounter++;
	std::string token;
	descr >> token;
	if (token.compare("{") != 0) {
		name = token;
		descr >> token;
		confirmParse(token, "{");
	} else {
		name = "no-name";
	}

	if (DEBUG)
		std::cout << "Parsing joint " << name << "-" << myCounter << std::endl;

	descr >> token;
	confirmParse(token, "OFFSET");
	descr >> offset[0] >> offset[1] >> offset[2];
	descr >> token;
	confirmParse(token, "CHANNELS");

	unsigned numChannels;
	descr >> numChannels;
	for (unsigned i = 0; i < numChannels; ++i) {
		descr >> token; // TODO actually get this info!!
	}

	descr >> token;
	while (token.compare("}") != 0) {
		if (token.compare("End") == 0) {
			descr >> token;
			confirmParse(token, "Site");
			descr >> token;
			confirmParse(token, "{");
			descr >> token;
			confirmParse(token, "OFFSET");
			boost::array<float, 3> offs;
			descr >> offs[0] >> offs[1] >> offs[2];
			children.push_back(SkeletonNode(offs));
			descr >> token;
			confirmParse(token, "}");
		} else if (token.compare("JOINT") == 0) {
			children.push_back(SkeletonNode(descr));
		} else {
			assert(false); // should be impossible
		}
		descr >> token;
	}

}

/* Use this constructor for leaf nodes.
 */
SkeletonNode::SkeletonNode(boost::array<float, 3> offsets) {
	myCounter = nodeCounter++;
	name = "leaf";
	if (DEBUG)
		std::cout << "Created " << name << "-" << myCounter << std::endl;
	offset = offsets;
}

SkeletonNode::~SkeletonNode() {
	if (DEBUG)
		std::cout << name << "-" << myCounter << " is now dying." << std::endl;
}

/* If this is a leaf then it has no endpoint .. we throw 0 */
boost::array<float, 3> SkeletonNode::getEndPoint() throw(int) {
	if (children.size() == 0) throw 0;
	else return children[0].offset;
}


void SkeletonNode::display() {
	if (children.size() == 0) return;

	glPushMatrix();
	// do all the drawing here
	glTranslatef(offset[0], offset[1], offset[2]);
	boost::array<float, 3> endP = getEndPoint();
    glBegin(GL_LINES);
       glVertex3f(0.0f, 0.0f, 0.0f);
	   glVertex3f(endP[0], endP[1], endP[2]);
    glEnd();
    std::cout << "Draw line (0,0,0) --> ("
    		<< endP[0] << ", " << endP[1] << ", " << endP[2] << ")." << std::endl;
    for (unsigned i = 0; i < children.size(); ++i) {
    	children[i].display();
    }

	glPopMatrix();
}


/* Prints the names of the subtree rooted at this node, with level number of
 * "- " thingies in front of it.
 */
void SkeletonNode::printNames(unsigned level) {
	for (unsigned i = 0; i < level; ++i) std::cout << "- ";
	std::cout << name << ": \t "
			<< offset[0] << ", "
			<< offset[1] << ", "
			<< offset[2] << ", " << std::endl;
	for (std::vector<SkeletonNode>::iterator it = children.begin();
											it != children.end(); ++it) {
		it->printNames(level+1);
	}
}


