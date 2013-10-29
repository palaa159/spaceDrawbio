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
    string name =  "Space Aubio Apon";
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
    maxArray = 300;
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
    ofEnableAlphaBlending();
	// draw pitch and vol text
	ofSetColor(255,255,255);
	dinFont.drawString( "pitch is : " + ofToString((int)pitch), 20,50);
    dinFont.drawString( "amplitude is : " + ofToString(vol), 20,70);
    dinFont.drawString( "other pitch is : " + ofToString((int)otherPitch), 20,90);
    dinFont.drawString( "other amplitude is : " + ofToString(otherVol), 20,110);
   
    // from algo2013, sinExample CirclePlusPath
    // my side
    float xorig = ofGetWidth()/2;
	float yorig = ofGetHeight()/2;
	float angle = ofGetElapsedTimef()*2;
	
	float x = xorig + circRadius * cos(angle);
	float y = yorig + circRadius * -sin(angle);
    
    ofVec2f pitchTemp;
	pitchTemp.x = x;
	pitchTemp.y = y;
    
    points.push_back(pitchTemp);
    circleRadius.push_back(vol);
	if (points.size() > maxArray){
		points.erase(points.begin());
	}
    if (circleRadius.size() > maxArray) {
        circleRadius.erase(circleRadius.begin());
    }
	
    ofFill();
    ofBeginShape();
	for (int i = 0; i < points.size(); i++){
//		ofVertex(points[i].x, points[i].y);
        ofSetColor(255,254,48,150);
        ofCircle(points[i].x, points[i].y, circleRadius[i]/2);
	}
	ofEndShape();
    
    // your side
    float yX = xorig + otherPitch/3 * -cos(angle);
    float yY = yorig + otherPitch/3 * sin(angle);
    
    ofVec2f yourPitchTemp;
    yourPitchTemp.x = yX;
    yourPitchTemp.y = yY;
    
    yourPoints.push_back(yourPitchTemp);
    yourCircleRadius.push_back(otherVol);
    if (yourPoints.size() > maxArray) {
        yourPoints.erase(yourPoints.begin());
    }
    if (yourCircleRadius.size() > maxArray) {
        yourCircleRadius.erase(yourCircleRadius.begin());
    }
	
    ofFill();
    ofBeginShape();
	for (int i = 0; i < yourPoints.size(); i++){
        //		ofVertex(points[i].x, points[i].y);
        ofSetColor(255,22,68,150);
        ofCircle(yourPoints[i].x, yourPoints[i].y, yourCircleRadius[i]/2);
	}
	ofEndShape();
    
    // record time when pitches match
    if (abs(pitch-otherPitch) >= 10 && abs(pitch-otherPitch) <= 40) {
        timer++;
        ofSetColor(255,22,68);
        dinFont.drawString( "MATCHING: " + ofToString(timer), 20,130);
        
    } else {
        timer = 0;
    }
    
    if(timer == 20){
        cout << "EXPLODE" << endl;
    }
    
    // check for proximity
    for(int i=0; i < points.size(); i++) {
        for(int j=0; j < yourPoints.size(); j++) {
            if(abs(ofDist(points[i].x, points[i].y, yourPoints[j].x, yourPoints[j].y)) <= 10) {
                ofSetColor(255,255,255,100);
                ofCircle(points[i].x, points[i].y, circleRadius[i]/2);
                ofCircle(yourPoints[i].x, yourPoints[i].y, yourCircleRadius[i]/2);
                float noise = ofNoise(points[i].x * 0.005, points[i].y * 0.005, ofGetElapsedTimef() * 0.1) * 15.0;
                points[i].x += ofNoise(noise);
                points[i].y += ofNoise(noise);
            }
        }
    } 
}
//--------------------------------------------------------------
void testApp::onMessage( Spacebrew::Message & msg ){
    if(msg.name == "SpaceAubio_pitch_receive") {
        otherPitch = ofToInt(msg.value);
        //cout << "other pitch is" << otherPitch << endl;
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
