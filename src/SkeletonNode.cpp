/*
 * SkeletonNode.cpp
 *
 *  Created on: 2012-10-16
 *      Author: david
 */


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
	std::string token;
	descr >> token;
	if (token.compare("{") != 0) {
		name = token;
		descr >> token;
		confirmParse(token, "{");
	} else {
		name = "no-name";
	}

	std::cout << "Parsing joint with name " << name << std::endl;

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
			float offs[3];
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
SkeletonNode::SkeletonNode(float offsets[]) {
	name = "leaf";
	std::copy(offsets, offsets+3, offset);
	// TODO test this!!!

}

SkeletonNode::~SkeletonNode() {
	std::cout << name << " is now dying." << std::endl;
}

/* Prints the names of the subtree rooted at this node, with level number of
 * "- " thingies in front of it.
 */
void SkeletonNode::printNames(unsigned level) {
	for (unsigned i = 0; i < level; ++i) std::cout << "- ";
	std::cout << name << std::endl;
	for (std::vector<SkeletonNode>::iterator it = children.begin();
											it != children.end(); ++it) {
		it->printNames(level+1);
	}
}


