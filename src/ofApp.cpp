#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(12);
    
    //----------------------------------------------------------
    // assign random position & fill array
    //----------------------------------------------------------
    population.reserve(population_size);
    for (size_t i = 0; i < population_size; ++i)
    {
        fly f;
        f.position = { ofRandom(ofGetWidth()), ofRandom(ofGetHeight()) };
        population.push_back(f);
    }
    
    //----------------------------------------------------------
    // allocate the grayscale image
    //----------------------------------------------------------
    surface.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_GRAYSCALE);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    
    //----------------------------------------------------------
    // generate greyscale noise image
    //----------------------------------------------------------
    //float noiseScale = ofMap(mouseX, 0, ofGetWidth(), 0, 0.1); // use the mouse to control the scale of the noise
    float noiseScale = 0.0033;
    float noiseVel = ofGetElapsedTimef()/2; // make it move
    
    unsigned char * pixels = surface.getPixels().getData();
    int w = ofGetWidth();
    int h = ofGetHeight();
    
    // loop through the with and the height of the image
    for(int y = 0; y < h; y++) {
        for(int x = 0; x < w; x ++) {
            int i = y * w + x;
            float noiseValue = ofNoise(x * noiseScale, y * noiseScale, noiseVel);
            pixels[i] = 255 * noiseValue;
        }
    }
    surface.update();
    
    //----------------------------------------------------------
    // calculate the best fitness
    //----------------------------------------------------------
    float best_fitness = std::numeric_limits<float>::max(); // retuns the maximum value
    
    size_t index = 0;
    
    for (auto& f : population)
    {
        //------------------------------------------------------
        // set the fitness to the brightest pixel by subtracting
        // the brightness from 255 so the best position will be
        // 0 or close to 0
        //------------------------------------------------------
        f.fitness = 255.0 - surface.getColor(f.position[0], f.position[1]).getBrightness();
        
        if (f.fitness < best_fitness)
        {
            best_fitness = f.fitness;
            index_best_fly = index;
        }
        ++index;
    }
    
    index = 0;
    size_t left = 0;
    size_t right = 0;
    
    //----------------------------------------------------------
    // topology ring to connect the first fly with the last
    //----------------------------------------------------------
    for (auto& f : population){
        
        if (index == 0)
        {
            left = population.size() - 1;
            right = index + 1;
        }
        else if (index == population.size() - 1)
        {
            left = index - 1;
            right = 0;
        }
        else
        {
            left = index - 1;
            right = index + 1;
        }
        
        //------------------------------------------------------
        // setting the best neighbour
        //------------------------------------------------------
        fly best_neigbour = (population[left].fitness < population[right].fitness) ? population[left] : population[right];
        
        //------------------------------------------------------
        // asign random positions for further exploration
        //------------------------------------------------------
        if(ofRandom(1.0) < disturbance_threshold){
            f.position[0] = ofRandom(0, ofGetWidth());
            f.position[1] = ofRandom(0, ofGetHeight());
        }else{
            f.position[0] = ofClamp(best_neigbour.position[0] + ofRandom(1.0) * (population[index_best_fly].position[0] - best_neigbour.position[0]), 0, ofGetWidth());
            f.position[1] = ofClamp(best_neigbour.position[1] + ofRandom(1.0) * (population[index_best_fly].position[1] - best_neigbour.position[1]), 0, ofGetHeight());
            
        }
        
        ++index;
    }
  //  cout << best_fitness << endl;
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    //----------------------------------------------------------
    // display the image
    //----------------------------------------------------------
    ofSetColor(255);
    surface.draw(0, 0);
    
    //----------------------------------------------------------
    // display the flies
    //----------------------------------------------------------
    ofSetColor(0);
    for (auto f : population){
        ofDrawCircle(f.position[0], f.position[1], 2);
    }
    
    //----------------------------------------------------------
    // display the best fly
    //----------------------------------------------------------
    ofSetColor(255, 0, 0);
    ofDrawCircle(population[index_best_fly].position[0], population[index_best_fly].position[1], 4);
}

