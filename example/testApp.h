#pragma once

#include "ofMain.h"
#include "ofxImageExportQueue.h"

#define NUM_PART 5000

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();
		void keyPressed(int key);

		ofxImageExportQueue * exporter;

		typedef struct particle{
			ofVec2f p;
			ofVec2f v;
			ofColor c;
		};
	
		particle particles[NUM_PART];
		bool recording;
		
};
