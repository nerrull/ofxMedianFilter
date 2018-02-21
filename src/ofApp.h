#pragma once

#include "ofMain.h"


struct valueOfInterest{
    int hue;
    u_char* pixelPointer;
    valueOfInterest(int h, u_char *pp){
        hue = h;
        pixelPointer =pp;
    }
};

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

    void doMedian();
    ofImage im;
    ofTexture tex;
    ofPixels *lastFrame;
    ofPixels *currentFrame;
    int * hueValues;
    int select =3;
    int bit_offset=0;
    int y_drift=0;
    int x_drift =0;



    int windowWidth;
    int windowHeight;
    int numPixels;
    int imageWidth;
    int imageHeight;

    vector<valueOfInterest> sortVector;
    u_char ** pixelPointerArray;
    int *offsets;

};

