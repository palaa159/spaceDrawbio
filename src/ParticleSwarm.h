//
//  ParticleSwarm.h
//  SpaceDrawbio
//
//  Initial swarm inspired by Bernardo Schorr.
//  Initial particle system inspired by Charley Whitney.
//
//

#pragma once
#include "ofMain.h"

class ParticleSwarm {
public:
    
    void setup(ofVec2f _pos, ofVec2f _dest);
    void update(int i, int &countOne, int &countTwo);
    void dissolve();
    void draw();
    
    
    ofVec2f prevPos, pos, vel, dest, size;
    float xenoRandomPos, xenoRandomDest;
    
    ofColor color;
    
    //bool triggerOne, triggerTwo, triggerThree;;
    
    int age, lifespan;
    bool bIsDead;
    
};