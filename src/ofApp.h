#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"

class ofApp : public ofBaseApp{

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
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    
    
    maxiOsc osc [5][8], oldOsc[7][5][8];
    maxiFilter filter;
    
    
    unsigned int m_sample_rate;
    unsigned int m_buffer_size;
    
    
        void audioOut(float *output, int buffer_size, int channels);
    
    
    int startTime, endTime, currentTime, filtFreq;
    
    bool oddSeries, filtRise;

    int harmonicSeries;
    
    float frequency[5], harmonics[5][7], oldFrequency[7][5], oldHarmonics[7][5][7];
    float midi[128];
    
    int a;
    
    
    
    
    
    
};
