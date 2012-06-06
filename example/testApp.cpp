#include "testApp.h"


void testApp::setup(){

	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofBackground(0, 0, 0, 255);
	glEnable(GL_POINT_SMOOTH);
	glPointSize(5);
	glLineWidth(3);
	
	recording = false;
	
	exporter = new ofxImageExportQueue( 2 ); // 2 threads
	
	float speed = 0.1;
	for(int i = 0; i < NUM_PART; i++){
		particles[i].p = ofVec2f( ofGetWidth()/2, ofGetHeight()/2 );
		particles[i].v = ofVec2f( ofRandom(-speed,speed), ofRandom(-speed,speed) );		
	}
}



void testApp::update(){

	float dt = 0.01666666f;
	
	//update particles
	float gain = 2;
	float delay = 0.01;
	for(int i = 0; i < NUM_PART; i++){		
		particles[i].v.x += gain * ofSignedNoise( ofGetElapsedTimef() + delay * i + mouseX );	
		particles[i].v.y += gain * ofSignedNoise( ofGetElapsedTimef() - delay * i + mouseY );	
		particles[i].p += dt * particles[i].v;
	}	
	
	exporter->update();	
}

void testApp::keyPressed(int key){
	recording = !recording;
}

void testApp::exit(){
	
	delete exporter; // this might crash on of0700 bc freeImage is de-inited before we can stop the queue
}


void testApp::draw(){

	//draw particles
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofMesh mesh;
	
	for(int i = 0; i < NUM_PART; i++){		
		mesh.addVertex( particles[i].p );
	}	

	ofSetColor(128);

	mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
	mesh.drawFaces();

	mesh.setMode(OF_PRIMITIVE_POINTS);
	mesh.drawFaces();

	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofSetColor(255);

	
	//capture current frame
	if (recording){
		ofImage img;
		img.setUseTexture(false);
		img.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		
		//add frame to exporter
		exporter->saveImage( img );
		
		//draw rec circle
		ofSetColor(255,0,0);
		ofCircle(ofGetWidth() - 30, 30, 15);
	}
	//draw exporter status
	exporter->draw( 10, 10);

	//draw msg
	ofSetColor(255);
	ofDrawBitmapString("press any key to " + (string)(recording ? "STOP" : "START") + " recording - " + ofToString(ofGetFrameRate()), 10, ofGetHeight() - 10);
	
}

