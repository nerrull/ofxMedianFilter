#pragma once

#include "ofMain.h"
#include "sortnetwork.cpp"
#include "ofShader.h"
#include "ofVec3f.h"
struct valueOfInterest{
    int hue;
    u_char* pixelPointer;
    valueOfInterest(int h, u_char *pp){
        hue = h;
        pixelPointer =pp;
    }
};



class valueCompare{
public:
    inline bool operator() (valueOfInterest i, valueOfInterest j) const
    {
        return (j.hue > i.hue);
    }
};

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void runShader();


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
    void texturedQuad(float x, float y, float width, float height, float s = 1.0, float t = 1.0);

    void doMedian();
    ofImage im;
    ofTexture tex;
    ofPixels *lastFrame;
    ofPixels *currentFrame;
    ofShader shader;
    int * hueValues;
    int select =4;
    int bit_offset=0;
    int y_drift=0;
    int x_drift =0;

    ofFbo fbo;
    ofFbo fbo2;
    int num_iterations;




    int windowWidth;
    int windowHeight;
    int numPixels;
    int imageWidth;
    int imageHeight;
    StaticSort<9, valueCompare> sort;

    vector<valueOfInterest> sortVector;
    u_char ** pixelPointerArray;
    int *offsets;

};

