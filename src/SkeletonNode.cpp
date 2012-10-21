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
		std::cout << "Parsing joint " << getDescr() << std::endl;

	descr >> token;
	confirmParse(token, "OFFSET");
	descr >> offset[0] >> offset[1] >> offset[2];
	descr >> token;
	confirmParse(token, "CHANNELS");

	descr >> channelNum;
	// we may assume that this line is either
	// CHANNELS 6 Xposition Yposition Zposition Zrotation Yrotation Xrotation
	// CHANNELS 3 Zrotation Yrotation Xrotation
	if (channelNum == 6) {
		descr >> token; confirmParse("Xposition", token);
		descr >> token; confirmParse("Yposition", token);
		descr >> token; confirmParse("Zposition", token);
		descr >> token; confirmParse("Zrotation", token);
		descr >> token; confirmParse("Yrotation", token);
		descr >> token; confirmParse("Xrotation", token);
	} else if (channelNum == 3) {
		descr >> token; confirmParse("Zrotation", token);
		descr >> token; confirmParse("Yrotation", token);
		descr >> token; confirmParse("Xrotation", token);
	} else {
		std::stringstream ss;
		ss << "CHANNELS " << channelNum;
		throw ParseException("CHANNELS [3|6]", ss.str());
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
SkeletonNode::SkeletonNode(boost::array<float, 3> const & offsets) {
	myCounter = nodeCounter++;
	name = "leaf";
	if (DEBUG)
		std::cout << "Created " << getDescr() << std::endl;
	offset = offsets;
	channelNum = 0;
}

std::string SkeletonNode::getDescr() {
	std::stringstream ss;
	ss << name << "-" << myCounter;
	return ss.str();
}

SkeletonNode::~SkeletonNode() {
	if (DEBUG)
		std::cout << getDescr() << " is now dying." << std::endl;
}

/* If this is a leaf then it has no endpoint .. we throw 0 */
boost::array<float, 3> SkeletonNode::getEndPoint() throw(int) {
	if (children.size() == 0) throw 0;
	else return children[0].offset;
}

/* Extracts the animation info for the next frame from the stream. */
void SkeletonNode::addAnimationFrame(std::ifstream& descr) {
	// if this is a leaf just return
	if (children.size() == 0) return;

	// extract info for this node
	double xPos, yPos, zPos;
	if (channelNum == 6) {
		descr >> xPos >> yPos >> zPos;
	}
	double xRot, yRot, zRot;
	descr >> zRot >> yRot >> xRot;
	if (DEBUG) std::cout << "Transformation matrix " << getDescr() << ", frame " << motion.size() << std::endl;
	if (channelNum == 6) {
		motion.push_back(MotionFrame(zRot, yRot, xRot, xPos, yPos, zPos));
	} else if (channelNum == 3) {
		motion.push_back(MotionFrame(zRot, yRot, xRot));
	} else {
		std::cerr << "channelNum of " << getDescr() << " is " << channelNum << std::endl;
		assert (false);
	}

	// now extract info for all the children
	for (std::vector<SkeletonNode>::iterator it = children.begin();
											it != children.end(); ++it) {
		it->addAnimationFrame(descr);
	}
}


/* Displays frame 'frame'. If the argument is -1, it displays the initial pose
 * (This is the default value). */
void SkeletonNode::display(int frame = -1) {
	if (children.size() == 0) return;

	glPushMatrix();
	// do all the drawing here
	glTranslatef(offset[0], offset[1], offset[2]);

	if (frame >= 0) {
		motion[frame].applyTransformation();
	}

	boost::array<float, 3> endP = getEndPoint();
    glBegin(GL_LINES);
       glVertex3f(0.0f, 0.0f, 0.0f);
	   glVertex3f(endP[0], endP[1], endP[2]);
    glEnd();
//    if (DEBUG) {
//        std::cout << "Draw line (0,0,0) --> ("
//        		<< endP[0] << ", " << endP[1] << ", " << endP[2] << ")." << std::endl;
//    }

    for (unsigned i = 0; i < children.size(); ++i) {
    	children[i].display(frame);
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

// generates the transformation matrix for this frame
void MotionFrame::genMatrix() {
	// TODO later we need to use quarternions!!!!!!
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); // make a copy
	glLoadIdentity(); // start from nothing
	if (channels == 6) glTranslatef(xPos, yPos, zPos); // add new translation
	glRotatef(zRot, 0.0, 0.0, 1.0);
	glRotatef(yRot, 0.0, 1.0, 0.0);
	glRotatef(xRot, 1.0, 0.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, modelTrans); // save new rotation
	glPopMatrix(); // reset modelview to original settings
	// TODO this needs testing

	if (DEBUG) printMatrix();

}

void MotionFrame::printMatrix() {
	std::cout << std::fixed;
	std::cout.precision(6);
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			std::cout << "\t" << modelTrans[i*4+j];
		}
		std::cout << std::endl;
	}
}

// applies the transformation matrix corresponding to this frame
void MotionFrame::applyTransformation() {
	glMultMatrixf(modelTrans);
	// TODO this needs testing
}




