//
//  Particle.cpp
//  algo_week_09_02_animation
//
//  Created by Bernardo Santos Schorr on 10/28/13.
//  (edited for personal use by Alex Samuel on 10/29/13)
//
//


#include "ParticleSwarm.h"

void ParticleSwarm::setup(ofVec2f _pos, ofVec2f _dest) {
    
    pos = _pos;
    dest= _dest;
    prevPos = pos;
    vel.set(0.01, 0);
    size.set (10, 10);
    
    xenoRandomPos = ofRandom(0.95, 0.97);
    xenoRandomDest = 1.0 - xenoRandomPos;
    
    color.set(0, 0);
    age = 0;
    lifespan = ofRandom(50, 150);
    
}

void ParticleSwarm::update(int i, int &countOne, int &countTwo) {
    
    float tempRandom = ofRandomf();
    
    prevPos = pos;
    pos = pos*xenoRandomPos + dest*xenoRandomDest;
    float colorSet = ofMap(pos.x, 0, dest.x, 0, 100);
    color.set(0, colorSet);
    
    if (dest.x-pos.x < 1.0) {
        countOne ++;
    }
    
}


void ParticleSwarm::dissolve(){
    
    // Generate some noise based on where we are with x and y.  The particle slows down due to our multiplying the
    // velocity by 0.97, but we still want a lot of noise motion.  for this reason we pass in something we know will keep moving.  Time!
    // This is 3D noise.
    float noise = ofNoise(pos.x * 0.005, pos.y*0.005, ofGetElapsedTimef() * 0.1) * 15.0;
    
    // How close is our particle to dying.  0.0 = newborn, 1.0 = death
    float agePct = 1.0 - ((float)age / (float)lifespan);
    
    // use sin and cos to give us some sweepy circular motion.  The closer we are to death, the more noise we add.
    pos += ofVec2f( cos(noise), sin(noise) ) * (1.0-agePct);
    pos += vel;
    vel *= 0.97;
    
    
    age++;
    
    if( age > lifespan ) {
        bIsDead = true;         // we've had a good life.
    }
};

void ParticleSwarm::draw() {
    
    ofSetColor(255, 75);
    ofCircle(pos, 1);
    
}