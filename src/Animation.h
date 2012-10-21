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

	// these next 3 should NOT change!
	unsigned frameNum;
	float frameTime;
	double totalTime;

	unsigned curFrame;
	float timeIntoAnim;

	bool animating;

	boost::posix_time::ptime lastTime;

public:
	Animation(char *filename) throw(ParseException);
	virtual ~Animation();

	std::string getFileName() {return filename;}
	float getFrameTime() {return frameTime;}
	void display();
	void addToTime(double timediff);

	void startAnim() { animating = true; lastTime = boost::posix_time::microsec_clock::universal_time();}
	void stopAnim() {animating = false;}
	void reset();
};

#endif /* ANIMATION_H_ */
