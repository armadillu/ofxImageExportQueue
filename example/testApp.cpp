#include "testApp.h"


void testApp::setup(){

	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofBackground(0, 0, 0, 255);
	glEnable(GL_POINT_SMOOTH);
	glPointSize(4);
	glLineWidth(3);
	
	recording = false;
	
	exporter = new ofxImageExportQueue( 2 ); // 2 threads
	
	float w = 15;
	for(int i = 0; i < NUM_PART; i++){
		float percent = (float)i / (NUM_PART - 1);		
		particles[i].p = ofVec2f( ofGetWidth()/2 + ofRandom(-w, w), ofGetHeight()/2 + ofRandom(-w, w));
		particles[i].c.setHsb( 255. * percent, 255, 200, 64);
	}
}


void testApp::update(){

	// update particles /////////////////////////////////////////////////////////////////////////////
	float dt = 0.01666666f;
	
	float timeScale = 0.15;
	float size = 700 * timeScale;
	float delay = 0.0009;
		
	for(int i = 0; i < NUM_PART; i++){		
		particles[i].v.x = size * ofSignedNoise( timeScale * ofGetElapsedTimef() + delay * i );	
		particles[i].v.y = size * ofSignedNoise( timeScale * ofGetElapsedTimef() - delay * i );	
		particles[i].p += dt * particles[i].v;
	}		
	// end particles /////////////////////////////////////////////////////////////////////////////
	
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
	ofMesh mesh;
	
	for(int i = 0; i < NUM_PART; i++){		
		mesh.addVertex( particles[i].p );
		mesh.addColor( particles[i].c );
	}	
		
	mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	mesh.drawFaces();
	
	//mesh.clearColors();
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofSetColor(16);
	mesh.setMode(OF_PRIMITIVE_POINTS);
	mesh.drawFaces();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);

	
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

