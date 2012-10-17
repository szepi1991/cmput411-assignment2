/*
 * main.cpp
 *
 *  Created on: 2012-10-16
 *      Author: david
 */

#include <iostream>
#include <boost/shared_ptr.hpp>

#include "Animation.h"
#include "ParseException.h"

using namespace std;

void setup(int argc, char **argv) throw (int) {

	if (argc != 2) {
		cerr << "ERROR: this program takes exactly 1 argument: the .obj file to load." << endl;
		throw 1;
	}

	cout << "Reading in file now." << endl;
	try {
		boost::shared_ptr<Animation> anim(new Animation(argv[1]));

		cout << "The name of the loaded file is " << anim->getFileName() << endl;

		int wait = 0;
		while (wait < (1 << 30)) { wait++; }
		wait = 0;
		while (wait < (1 << 30)) { wait++; }
		// now Animation should die..

	} catch (ParseException& e) {
		cerr << e.what() << endl;
		throw 2;
	}

}

int main(int argc, char **argv) {



	// Initialize.
	try {
		setup(argc, argv);
	} catch (int e) {
		cerr << "The program is going to terminate now." << endl;
	}

	cout << "Exit!" << endl;

	return 0;
}



