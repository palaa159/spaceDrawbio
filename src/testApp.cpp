#include "testApp.h"
#include "stdio.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
   	ofBackground(0,0,0);
    ofSetBackgroundAuto(true);
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
    
    //create particles (adapted from Algo)
    newPos.set(0,0);
    // make 50 particles up front!
    for( int i=0; i<50; i++ ){
        addParticle(newPos);
    }
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
    
    
    //Loop through and delete a particle using iterators and vectors (from Algo)

    for( vector<Particle>::iterator it=pList.begin(); it!=pList.end(); ){
        it->update();
        
        if( it->bIsDead ){
            it = pList.erase(it);   // When we erase one, it returns the next particle automatically.  It's done the "it++" for us!
            ofLog( OF_LOG_NOTICE, "size is " + ofToString(pList.size()) );
        }else {
            it++;
        }
    }
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
    // my side -------------------------------------------------------------- MY SIDE
    float xorig = ofGetWidth()/2;
	float yorig = ofGetHeight()/2;
	float angle = ofGetElapsedTimef()*2;
	
	float x = xorig + circRadius * cos(angle);
	float y = yorig + circRadius * -sin(angle);
    
    ofVec2f pitchTemp;
	pitchTemp.x = x;
	pitchTemp.y = y;
    
    myPoints.push_back(pitchTemp);
    myCircleRadius.push_back(vol);
	if (myPoints.size() > maxArray){
		myPoints.erase(myPoints.begin());
	}
    if (myCircleRadius.size() > maxArray) {
        myCircleRadius.erase(myCircleRadius.begin());
    }
	
    ofFill();
    ofBeginShape();
	for (int i = 0; i < myPoints.size(); i++){
//		ofVertex(points[i].x, points[i].y);
        int myC = ofMap(myCircleRadius[i], 0, 70, 0, 255);
        ofSetColor(255,myC,48,150);
//        ofCircle(myPoints[i].x, myPoints[i].y, myCircleRadius[i]/2);
        ofSetLineWidth(myCircleRadius[i]/8);
        if(i > 1) {
        ofLine(myPoints[i-1], myPoints[i]);
        }
	}
	ofEndShape();
    
    // your side -------------------------------------------------------------- YOUR SIDE
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
        //	ofVertex(points[i].x, points[i].y);
        int yourC = ofMap(yourCircleRadius[i], 0, 70, 0, 255);
        ofSetColor(yourC,208,208,150);
        ofSetLineWidth(yourCircleRadius[i]/8);
        if(i > 1) {
            ofLine(yourPoints[i-1], yourPoints[i]);
        }
        newPos.set(yourPoints[i].x, yourPoints[i].y);
	}
	ofEndShape();
    
    // PARTICLE SYSTEM -------------------------------------------------------------- PARTICLE
    ofSetColor(255,255,255, 200);
    //drawParticle from Algo
    for( vector<Particle>::iterator it = pList.begin(); it!=pList.end(); it++){
        it->draw();
    }
    
    // record time when pitches match
    if (abs(pitch-otherPitch) >= 10 && abs(pitch-otherPitch) <= 40) {
        timer++;
        ofSetColor(255,22,68);
        dinFont.drawString( "MATCHING: " + ofToString(timer), 20,130);
        
    } else {
        timer = 0;
    }
    
    if(timer >= 10 && timer <= 12){
        cout << "EXPLODE" << endl;
        cout << "newPos is: " << newPos << endl;
        
        for( int i=0; i<60; i++ ){
            newPos.set(x, y);
            addParticle(newPos);
        }
        // explode dots away
//        for(int i=0; i < points.size();i++) {
//            float noise = ofNoise(points[i].x * 0.005, points[i].y * 0.005, ofGetElapsedTimef() * 0.1) * 15.0;
//            points[i] += ofVec2f(cos(noise), sin(noise))/2;
//        }
        
    }
    
    // check for proximity ---------------------------------------------------------------------
    for(int i=0; i < myPoints.size(); i++) {
        for(int j=0; j < yourPoints.size(); j++) {
            if(abs(ofDist(myPoints[i].x, myPoints[i].y, yourPoints[j].x, yourPoints[j].y)) <= 10) {
                ofSetColor(255,255,255,150);
                ofCircle(myPoints[i].x, myPoints[i].y, myCircleRadius[i]/8);
                ofCircle(yourPoints[i].x, yourPoints[i].y, yourCircleRadius[i]/8);
                float noise = ofNoise(myPoints[i].x * 0.005, myPoints[i].y * 0.005, ofGetElapsedTimef() * 0.1) * 15.0;
                myPoints[i] += ofVec2f(cos(noise), sin(noise))/ ofRandom(4,10);
                yourPoints[i] += ofVec2f(cos(noise), sin(noise))/ ofRandom(4,10);
            }
        }
    } 
    ofSetColor(0, 0, 0, 10);
    ofRect(0,0,ofGetWidth(),ofGetHeight());

}

//--------------------------------------------------------------
void testApp::addParticle(ofVec2f currentPos){
    
    // we'll pick a random direction for our particle to move initially
    rVel = ofVec2f( ofRandom(1.0), ofRandom(1.0) ) * 10.0 - 5.0;
    newPos = currentPos;
    p.setup( rVel, newPos );
    pList.push_back( p );
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
    // add 60 more particles on click!

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
