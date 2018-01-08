#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    //Midi to Frequency
    a = 440;
    for (int i = 0; i < 128; i++) {
        
        midi[i] = 13.75 * pow(2.0,(i - 9.0) / 12.0);
        
    }
    
    
    //3 Available harmonic series' - harmonics 1-8 if 0, harmonics 1-15 (only odds) if 1, harmonics 1-14 (only evens excluding primary harmonic) if 3
    harmonicSeries = 0;
    oddSeries = false;
    
    m_sample_rate = 44100;
    m_buffer_size = 512;
    
    
    ofSoundStreamSetup(2, 0, m_sample_rate, m_buffer_size, 4);
    
    
    //Timer Stuff
    startTime = ofGetElapsedTimeMillis();
    endTime = 5000;
    
    for(int i = 0; i < 5; i++){
        
        frequency[i] = midi[(int)ofRandom(1, 127)];
        
        for(int j = 0; j < 7; j++){
            
            if(harmonicSeries <= 33){
            harmonics[i][j] = frequency[i] * (j+2);
            }
            else if(harmonicSeries > 33 && harmonicSeries <= 66){
                harmonics[i][j] = frequency[i] * ((2*j) + 3);
            }
            else {
                harmonics[i][j] = frequency[i] * ((2*j) + 2);
            }
        }
                
    }

    
    filtRise = true;
    filtFreq = 501;
}

//--------------------------------------------------------------
void ofApp::update(){

    currentTime = ofGetElapsedTimeMillis() - startTime;
    
    if(currentTime >= endTime){
        
        harmonicSeries = ofRandom(100);
        
        
        //Loop backwards through array to set most recently selected frequencies last
        //Looping forwards would set every old frequency to the current frequency.
        //First set the prime frequency
        for(int i = 6; i > 0;  i--){
            
            for(int j = 0; j < 5; j++){
                
                oldFrequency[i][j] = oldFrequency[i-1][j];
                
                if(i == 1){
                oldFrequency[0][j] = frequency[j];
                    }
                
                
                //Set the harmonics
                for(int k = 0; k < 7; k++){
                    
                    if(harmonicSeries <= 33){
                        oldHarmonics[i][j][k] = oldFrequency[i][j] * (k+2);
                    }
                    else if(harmonicSeries > 33 && harmonicSeries <= 66) {
                            oldHarmonics[i][j][k] = oldFrequency[i][j] * ((2*k) + 3);
                    }
                    else
                        oldHarmonics[i][j][k] = oldFrequency[i][j] * ((2*k) + 2);
                }
            }
            

        }

        for (int i = 0; i < 5; i ++){
        
         //Change the current frequencies selected
        frequency[i] = midi[(int)ofRandom(1, 127)];
            
            //Set the harmonics
            for(int j = 0; j < 7; j++){
                
                if(harmonicSeries <= 33 ){
                    harmonics[i][j] = frequency[i] * (j+2);
                }
                else if(harmonicSeries > 33 && harmonicSeries <= 66){
                    harmonics[i][j] = frequency[i] * ((2*j) + 3);
                }
                else {
                    harmonics[i][j] = frequency[i] * ((2*j) + 2);

                }
            }
            
            
        }
        
        startTime = ofGetElapsedTimeMillis();
        
    }
    
    if(filtFreq <= 500){
        filtRise = true;
    }
    if(filtFreq >= 10000){
        filtRise = false;
    }
    if(filtRise == true){
        filtFreq +=3;
       // ofLog() << "Filt Change";
    }
    if(filtRise == false){
        filtFreq-=3;;
     //   ofLog() << "Filt Change";
        }
    
    ofLog() << "Filt Freq = " << filtFreq;
    }



//--------------------------------------------------------------
void ofApp::draw(){

}
void ofApp::audioOut(float *output, int buffer_size, int channels) {
    
    
    double harmonicMix[5], oldHarmonicMix[7][5];
    double mixL[8], oldMixL[7], currentMixL, filtMixL;
    double mixR[8], oldMixR[7], currentMixR, filtMixR;
    double osc_value[5][8];
    double oldOsc_value[7][5][8];
    
    for (unsigned int i = 0; i < buffer_size; ++i) {
       
        for(int l = 0; l < 7; l++){
            for(int j = 0; j < 5; j++){
            
                //Ensures osc_value is only set once 
                if(l==0){
                    osc_value[j][0] = osc[j][0].sinewave(frequency[j]);
                    }
                oldOsc_value[l][j][0] = oldOsc[l][j][0].sinewave(oldFrequency[l][j]);
                
                for(int k = 1; k < 8; k++){
                
                    if(l == 0){
                        osc_value[j][k] = osc[j][k].sinewave(harmonics[j][k-1]);
                        }
                    oldOsc_value[l][j][k] = oldOsc[l][j][k].sinewave(oldHarmonics[l][j][k-1]);
                    }
                
        }
    }
    
        for(int l = 0; l < 7; l++){
            for(int j = 0; j < 5; j++){
        
                if(l == 0){
                    harmonicMix[j] = osc_value[j][0] * 0.25; + osc_value[j][1] * 0.2 + osc_value[j][2] * 0.1 + osc_value[j][3] * 0.2 + osc_value[j][4] * 0.05 + osc_value[j][5] * 0.1 + osc_value[j][6] * 0.05 + osc_value[j][7] * 0.05;
                }
                
                oldHarmonicMix[l][j] = oldOsc_value[l][j][0] * 0.25; + oldOsc_value[l][j][1] * 0.2 + oldOsc_value[l][j][2] * 0.1 + oldOsc_value[l][j][3] * 0.2 + oldOsc_value[l][j][4] * 0.05 + oldOsc_value[l][j][5] * 0.1 + oldOsc_value[l][j][6] * 0.05 + oldOsc_value[l][j][7] * 0.05;
            }
        }
        
        for(int j = 1; j < 8; j++){
            
            mixL[j] = oldHarmonicMix[j][0] * 0.2 + oldHarmonicMix[j][1] * 0.2 + oldHarmonicMix[j][2] * 0.2 + oldHarmonicMix[j][3] * 0.2 + oldHarmonicMix[j][4] * 0.2;
            
            mixR[j] = oldHarmonicMix[j][0] * 0.2 + oldHarmonicMix[j][1] * 0.2 + oldHarmonicMix[j][2] * 0.2 + oldHarmonicMix[j][3] * 0.2 + oldHarmonicMix[j][4] * 0.2;
            
        }
        
        
        mixL[0] = harmonicMix[0] * 0.2 + harmonicMix[1] * 0.2 + harmonicMix[2] * 0.2 + harmonicMix[3] * 0.2 + harmonicMix[4] * 0.2;
        mixR[0] = harmonicMix[0] * 0.2 + harmonicMix[1] * 0.2 + harmonicMix[2] * 0.2 + harmonicMix[3] * 0.2 + harmonicMix[4] * 0.2;

       
        currentMixL = mixL[0] * 0.3 + mixL[1] * 0.25 + mixL[2] * 0.2 + mixL[3] * 0.1 + mixL[4] * 0.07 + mixL[5] * 0.04 + mixL[6] * 0.025 + mixL[7] * 0.015;
        currentMixR = mixR[0] * 0.3 + mixR[1] * 0.25 + mixR[2] * 0.2 + mixR[3] * 0.1 + mixR[4] * 0.07 + mixR[5] * 0.04 + mixR[6] * 0.025 + mixR[7] * 0.015;
        
        
        filtMixL = filter.lores(currentMixL, filtFreq, 5);
        filtMixR = filter.lores(currentMixR, filtFreq, 5);

        output[i * channels] = filtMixL;
        output[i * channels + 1] = filtMixR;
        
    }
    
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
