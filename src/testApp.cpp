#include "testApp.h"
#include "stdio.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
   	ofBackground(0,0,0);
	
	// setup of sound input
	ofSoundStreamSetup(0, 2, this, 44100, 512, 4);
	left = new float[512];
	right = new float[512];
	
	AA.setup();
	
	dinFont.loadFont("DIN.otf", 10);
    
    
    // spacebrew setup
    string host = "75.101.237.143"; // "localhost";
    string name =  "Space Aubio At New York";
    string description =  "thisIsJustATest";
    spacebrew.addPublish("SpaceAubio_pitch_send", "range");
    spacebrew.addPublish("SpaceAubio_volume_send", "range");
    spacebrew.addSubscribe("SpaceAubio_pitch_receive", "range");
    spacebrew.addSubscribe("SpaceAubio_volume_receive", "range");
    spacebrew.connect( host, name, description );
    Spacebrew::addListener(this, spacebrew);
    // background
    bBackgroundOn   = false;
    otherPitch = 0;
    otherRadius = 0;
    
    // for circle
    ofSetCircleResolution(100);
    circRadius = 10;
}

//--------------------------------------------------------------
void testApp::update(){
    
//    pitch = 0.05*AA.pitch + 0.95*pitch;
    vol = (int)(0.05* AA.amplitude * 10000) + (int)(0.95*(0));
    pitch = 0.05*AA.pitch + 0.95*pitch;
    
    if( spacebrew.isConnected()){
        spacebrew.sendRange("SpaceAubio_pitch_send", pitch);
        spacebrew.sendRange("SpaceAubio_volume_send", vol);
        otherRadius = otherPitch;
    }
    circRadius = pitch/3;
    dotRadius = vol;
    
}

//--------------------------------------------------------------
void testApp::draw(){
	// draw pitch and vol text
	ofSetColor(255,255,255);
	dinFont.drawString( "pitch is : " + ofToString((int)pitch), 20,50);
    dinFont.drawString( "amplitude is : " + ofToString(vol), 20,70);
    dinFont.drawString( "other pitch is : " + ofToString((int)otherPitch), 20,90);
    dinFont.drawString( "other amplitude is : " + ofToString(otherVol), 20,110);
   
    // from algo2013, sinExample CirclePlusPath
    float xorig = ofGetWidth()/2;
	float yorig = ofGetHeight()/2;
	float angle = ofGetElapsedTimef()*2;
	
	float x = xorig + circRadius * cos(angle);
	float y = yorig + circRadius * -sin(angle);
    
    ofPoint pitchTemp;
	pitchTemp.x = x;
	pitchTemp.y = y;
    
    points.push_back(pitchTemp);
    circleRadius.push_back(vol);
	if (points.size() > 50){
		points.erase(points.begin());
	}
    if (circleRadius.size() > 50) {
        circleRadius.erase(circleRadius.begin());
    }
    
    ofSetColor(255,255,255);
	
    ofFill();
    ofBeginShape();
	for (int i = 0; i < points.size(); i++){
//		ofVertex(points[i].x, points[i].y);
        ofCircle(points[i].x, points[i].y, circleRadius[i]);
	}
	ofEndShape();
}
//--------------------------------------------------------------
void testApp::onMessage( Spacebrew::Message & msg ){
    if(msg.name == "SpaceAubio_pitch_receive") {
        otherPitch = ofToInt(msg.value);
        cout << "other pitch is" << otherPitch << endl;
    }
    if(msg.name == "SpaceAubio_volume_receive") {
        otherVol = ofToInt(msg.value);
    }
}


//--------------------------------------------------------------

void testApp::audioReceived (float * input, int bufferSize, int nChannels){
	
    // samples are "interleaved"
	for (int i = 0; i < bufferSize; i++){
		left[i] = input[i*2];
		right[i] = input[i*2+1];
	}
    
	AA.processAudio(left, bufferSize);
	
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
    
}
