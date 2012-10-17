/*
 * Animation.cpp
 * Reads a Biovision .bvh file describing a simplified figure skeleton
 * represented by a hierarchy of line segments (limbs) connected by joints.
 * Allows for the playing of this animation
 *
 *  Created on: 2012-10-16
 *      Author: david
 */

#include "Animation.h"
#include "SkeletonNode.h"
#include "tools.h"

#include <fstream>
#include <sstream>



Animation::Animation(char *filename) throw(ParseException) {


	std::ifstream infile(filename);
	// read stuff in
	std::string word;
	while (infile >> word && word.compare("HIERARCHY") != 0) {
	}
	// now description starts
	infile >> word;
	assert( word.compare("ROOT") == 0 );
	root.reset(new SkeletonNode(infile));

	// TODO also read in the animation code

	infile.close();

	this->filename = filename;

	std::cout << "The tree structure we read in is:" << std::endl;
	root->printNames(0);

}

Animation::~Animation() {
	// because of shared_ptr the node that root is pointed to gets deleted.
}

void Animation::display() {
//	std::cout << "In theory things are drawn now" << std::endl;
	root->display();
}


