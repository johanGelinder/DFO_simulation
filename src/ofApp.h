#pragma once

#include "ofMain.h"
#include <array>

struct fly
{
    //std::array<double, 2> position;
    vector<float> position;
    float fitness;
};

//-------------------------------------------
class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    
private:
    
    std::vector<fly> population;
    const std::size_t population_size = 300;
    
    ofImage surface;
    size_t index_best_fly;
    float disturbance_threshold = 0.1;

};
