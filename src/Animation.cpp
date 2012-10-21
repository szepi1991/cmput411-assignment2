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
	infile >> frameTime;

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
		confirmParse("", word);
	}
	infile.close();

	this->filename = filename;
	timeIntoAnim = 0;
	curFrame = -1;
	totalTime = (double) frameNum * (double) frameTime;
	animating = false;

}

Animation::~Animation() {
	// because of shared_ptr the node that root is pointed to gets deleted.
}

// timediff is in milliseconds!!
void Animation::addToTime(double timeDiff) {
	if (DEBUG) 	std::cout << "Elapsed time (ms): " << timeDiff << std::endl;

	timeDiff /= SECtoMSEC;
	timeIntoAnim += timeDiff;
	while (timeIntoAnim > totalTime) timeIntoAnim -= totalTime;
	// figure out which frame this corresponds to:
	// TODO at this point we ask for an actual frame.. later this will not be
	// like so (interpolate!)
	curFrame = ( timeIntoAnim / frameTime ) + 0.5;
	if (DEBUG)
		std::cout << "curtime = " << timeIntoAnim << ", frametime = " << frameTime
			<< " --> curFrame = " << curFrame << std::endl;
	// it might happen that due to rounding curFrame == frameNum.. make it go back to 0
	if (curFrame == frameNum) curFrame = 0;

	assert (curFrame < frameNum);
}

// reset to initial pose
void Animation::reset() {
	animating = false;
	timeIntoAnim = 0; curFrame = -1;
}


// displays the current frame (that has been already calculated from curTime)
void Animation::display() {

	boost::posix_time::ptime curTime = boost::posix_time::microsec_clock::universal_time();
	if (animating) {
		boost::posix_time::time_duration dur = curTime - lastTime;
		addToTime( dur.total_milliseconds() ); // automatically converted to double
	}
	lastTime = curTime;

	if (DEBUG) std::cout << "Drawing Frame " << curFrame << std::endl;
	for (unsigned i = 0; i < roots.size(); ++i)
		roots[i].display(curFrame);
}


