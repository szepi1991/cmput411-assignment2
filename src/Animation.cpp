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
#include <cmath>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif



Animation::Animation(char *filename) throw(ParseException) {

	std::ifstream infile(filename);
	// read stuff in
	std::string word;
	while (infile >> word && word.compare("HIERARCHY") != 0) {
	}
	// now description starts
	infile >> word;
//	assert( word.compare("ROOT") == 0 );
//	root.reset(new SkeletonNode(infile))
//	unsigned rootNum = 0;
	while (word.compare("ROOT") == 0) {
		roots.push_back(SkeletonNode(infile));
		std::cout << "The tree structure we read in is:" << std::endl;
		(roots.end()-1)->printNames(0);
		infile >> word;
	}

	if (word.compare("MOTION") != 0)
		throw ParseException("MOTION", word);

	// parse the motion stuff
	infile >> word;
	confirmParse("Frames:", word);
	infile >> frameNum;
	infile >> word;
	confirmParse("Frame", word);
	infile >> word;
	confirmParse("Time:", word);
	infile >> stdFrameTime;

	// assume that the animation for the 2 roots are interleaved
	for (unsigned f = 0; f < frameNum; ++f) {
		for (std::vector<SkeletonNode>::iterator rootIt = roots.begin();
											rootIt != roots.end(); ++rootIt) {
			rootIt->addAnimationFrame(infile);
		}
	}

	// there should be nothing more in the file
	while (!infile.eof()) {
		std::getline(infile, word);
		if (word.begin() != remove_if(word.begin(), word.end(), isspace)) {
			std::cerr << "Unexpected term '" << word << "' at the end of the bvh file" << std::endl;
		}
	}
	infile.close();

	this->filename = filename;
	animating = false;

	stdFPS = floor((1.0 / stdFrameTime) + 0.5);
	virtFPS = stdFPS;

	curFrameWhole = -1;
	curFrameFrac = curFrameWhole;
}

Animation::~Animation() {
	// because of shared_ptr the node that root is pointed to gets deleted.
}

// timediff is in milliseconds!!
void Animation::addToTime(double timeDiff) {
	if (DEBUG) 	std::cout << "Elapsed time (ms): " << timeDiff << std::endl;
	timeDiff /= SECtoMSEC; // now timeDiff is in seconds

	// calculate which frame we moved ahead to - depends on virtual fps
	curFrameFrac += timeDiff * virtFPS;
	while (curFrameFrac > frameNum) curFrameFrac -= frameNum; // TODO make it >= ?
	while (curFrameFrac < 0) curFrameFrac += frameNum; // so negative case is also handled

	// figure out which real frame this corresponds to:
	// TODO at this point we ask for an actual frame.. later this will not be like this (interpolate!)
	curFrameWhole = ( curFrameFrac ) + 0.5;

	// it might happen that due to rounding curFrame == frameNum.. make it go back to 0
	if (curFrameWhole == frameNum) curFrameWhole = 0;

	assert (curFrameWhole < frameNum);
}

// reset to initial pose
void Animation::reset() {
	animating = false;
	curFrameWhole = -1; curFrameFrac = -1;
	virtFPS = stdFPS;
}

void Animation::outputBVH(std::ostream& out) {
	out << std::fixed;
	out.precision(5);
	// first output the tree
	out << "HIERARCHY" << std::endl;
	for (std::vector<SkeletonNode>::iterator rootIt = roots.begin();
										rootIt != roots.end(); ++rootIt) {
		rootIt->printTreeBVH(out, 0);
	}

	// now output the animation code
	out.precision(7);
	out << "MOTION" << std::endl;
	out << "Frames: " << frameNum << std::endl;
	out << "Frame Time: " << stdFrameTime << std::endl;
	out.precision(4);
	for (unsigned f = 0; f < frameNum; ++f) {
		for (std::vector<SkeletonNode>::iterator rootIt = roots.begin();
											rootIt != roots.end(); ++rootIt) {
			rootIt->printFrameBVH(out, f);
		}
		out << std::endl;
	}
}

// displays the current frame (that has been already calculated from curTime)
void Animation::display() {

	boost::posix_time::ptime curTime = boost::posix_time::microsec_clock::universal_time();
	if (animating) {
		boost::posix_time::time_duration dur = curTime - lastTime;
		addToTime( dur.total_milliseconds() ); // automatically converted to double
	}
	lastTime = curTime;

	if (DEBUG) std::cout << "Drawing Frame " << curFrameWhole << std::endl;
	for (unsigned i = 0; i < roots.size(); ++i)
		roots[i].display(curFrameWhole);
}


