/*
 * Utility.h
 *
 *  Created on: Dec 6, 2019
 *      Author: chwhite
 */

#ifndef UTILITY_H_
#define UTILITY_H_

class Utility {
public:
	Utility(char* inFileName);
	virtual ~Utility();
	char* openFile();
	void writeFile(char* message);


private:
	char *fileName;
};

void interface();

#endif /* UTILITY_H_ */
