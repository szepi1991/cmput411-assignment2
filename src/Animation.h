/*
 * Animation.h
 *
 *  Created on: 2012-10-16
 *      Author: david
 */

#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

#include "SkeletonNode.h"
#include "ParseException.h"

class Animation {
private:
	std::string filename;
//	boost::shared_ptr<SkeletonNode> root;
	std::vector<SkeletonNode> roots;

	// these next 2 should NOT change!
	unsigned frameNum;
	float stdFrameTime; // in seconds

	float curFrameFrac; // which frame are we at exactly
	unsigned curFrameWhole; // basically curFrameFrac rounded

	bool animating;
	float stdFPS;
	float virtFPS;

	boost::posix_time::ptime lastTime;

public:
	Animation(char *filename) throw(ParseException);
	virtual ~Animation();

	std::string getFileName() {return filename;}
	float getStdFrameTime() {return stdFrameTime;}
	float getVirtualFPS() { return virtFPS; }
	void display();
	void addToTime(float timediff);

	void startAnim() { animating = true; lastTime = boost::posix_time::microsec_clock::universal_time();}
	void stopAnim() {animating = false;}
	void reset();

	void addFPS(float diff) {virtFPS += diff;}
};

#endif /* ANIMATION_H_ */
