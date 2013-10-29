#pragma once

#include "ofMain.h"
#include "ofxSpacebrew.h"
#include "ofxAubioAnalyzer.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);


    	void audioReceived		(float * input, int bufferSize, int nChannels);
    
        float * left;
        float * right;
    
        ofxAubioAnalyzer AA;
        ofTrueTypeFont dinFont;
    
        // create your spacebrew object
        Spacebrew::Connection spacebrew;
        void onMessage( Spacebrew::Message & msg );
        int pitch;
        float vol;
        float otherVol;
    
        bool bBackgroundOn;
        int otherPitch;
        int otherRadius;
    
        vector <ofPoint> points;
        vector <float> circleRadius;
        float circRadius;
        float dotRadius;
    
        int maxArray;
    
        vector <ofPoint> yourPoints;
        vector <float> yourCircleRadius;
    
    float timer;
    
};
