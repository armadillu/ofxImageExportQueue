/*
 *  ofxImageExportQueue.h
 *
 *  Created by Oriol Ferrer Mesià on 25/03/11.
 *  Copyright 2011 uri.cat. All rights reserved.
 *
 */


#pragma once

#include "ofMain.h"
#include "WorkQueue.h"
#include "DedicatedMultiQueue.h"

class SaveImageWorkUnit : public GenericWorkUnit{

	public:

	ofImage img;
	string name;

	SaveImageWorkUnit( ofImage i, string fileName ){
		img = i;
		name = fileName;
	};
	
	virtual void process(){		
		setPercentDone(0);
		img.saveImage(name);
		setPercentDone(1);
	}
};


class ofxImageExportQueue {

	public:

		ofxImageExportQueue(int numThreads_ = 3){
			
			numThreads = numThreads_;
			imgCounter = 0;
			q1 = new DedicatedMultiQueue(numThreads);
			q1->setVerbose(false);
			q1->setIndividualWorkerQueueMaxLen(2);
			q1->setMaxPendingQueueLength(1000);
			q1->setRestTimeMillis(1);
		};

	
		~ofxImageExportQueue(){
			delete q1;
		};

		void setRestTime(int milis){
			q1->setRestTimeMillis(milis);
		}
	
		void update(){

			q1->update();
			//collect the NULL results, just to clear memory....
			for (int i = 0; i < numThreads ; i++){
				GenericWorkUnit * w = q1->retrieveNextProcessedUnit();
				if (w != NULL)
					delete w;
			}
		}

	
		void draw(int x, int y){
			q1->draw(x, y, 10);
		}

	
		bool saveImage(ofImage &i, string fileName = ""){

			bool tex = i.isUsingTexture();
			i.setUseTexture(false);			
			if (fileName.length() == 0){
				fileName = ofToString(imgCounter) + ".png";
			}
			
			SaveImageWorkUnit * w1 = new SaveImageWorkUnit( i, fileName );
			bool added = q1->addWorkUnit(w1);
			if (!added){ //we drop that frame
				delete w1;
			}else{
				imgCounter++;
			}
			i.setUseTexture(tex);
			return added;
		};
	
		int getPending(){ return  q1->getPendingQueueLength(); }
	
	private:
	
		DedicatedMultiQueue * q1;	
		int numThreads;
		int imgCounter;

};