#include "ofApp.h"



int toHue(u_char* pixelPointer){
    u_char r = pixelPointer[0];
    u_char g = pixelPointer[1];
    u_char b = pixelPointer[2];
    u_char max = MAX(MAX(r,g),b);
    u_char min = MIN(MIN(r,g),b);
    float hue;

    if (max ==r){
        hue = (g - b) /  (float)(max - min);
    }
    if (max ==g){
        hue = 2.f + (b - r) /  (float)(max - min);
    }

    if (max ==b){
        hue =4.f+ (r - g) / (float)(max - min);
    }
    if (hue < 0) hue = hue + 6;
    int hue_int =(int) (hue*255.0f/6.0f);
    int col = (hue_int<<24&0xff000000) |(r<<16&0x00ff0000)|(g<<8&0x0000ff00)|(b&0x000000ff);
    return col;

}

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(0);
    im.loadImage("test.jpg");
    im.update();
    lastFrame = &im.getPixels();
    currentFrame = new ofPixels(*lastFrame);
    num_iterations = 10;


    imageWidth = im.getWidth();
    imageHeight = im.getHeight();
    windowWidth =3;
    windowHeight= 3;
    numPixels = windowWidth*windowHeight;
    offsets = new int[windowHeight*windowWidth];

    pixelPointerArray = new u_char*[windowHeight*windowWidth];

    int index = 0;
    for(int w_y = 0; w_y < windowHeight; w_y += 1) {
        for(int w_x = 0; w_x < windowWidth; w_x += 1) {
            offsets[index]= w_x*3+ imageWidth*w_y*3;
            sortVector.push_back(valueOfInterest(0,0));
            index++;
        }
    }
    hueValues = new int[imageWidth*imageHeight];

    int corner_pixel_index =0;
    for(int y = 0; y < imageHeight; y += 1) {
        for(int x = 0; x < imageWidth; x += 1) {
            int realPixelIndex = corner_pixel_index*3;
            hueValues[corner_pixel_index]=toHue(&(lastFrame->getData()[realPixelIndex]));
            corner_pixel_index++;
        }
    }

    shader.load("median");

    fbo.allocate(imageWidth, imageHeight);
    fbo.begin();
    ofClear(0,0,0,255);
    fbo.end();

    fbo2.allocate(imageWidth, imageHeight);
    fbo2.begin();
    ofClear(0,0,0,255);
    fbo2.end();





}

//--------------------------------------------------------------
void ofApp::update(){

    uint64_t time = ofGetElapsedTimeMillis();
    fbo.begin();


    im.draw(0,0);
    fbo.end();


    for (int i = 0; i<num_iterations;i++){
       if (i%2==0){
           fbo2.begin();
           shader.begin();
           shader.setUniform1i("index", select);
           //shader.setUniformTexture("tex0", fbo.getTexture(), 0);
           //shader.setUniform1i("mode", 0);

           fbo.draw(0,0);
           shader.end();
           fbo2.end();

       }

       else {
           fbo.begin();
           shader.begin();
           shader.setUniform1i("index", select);
           //shader.setUniformTexture("tex0", fbo2.getTexture(), 0);
           //texturedQuad(0, 0, fbo2.getWidth(), fbo2.getHeight());
           //shader.setUniform1i("mode", 0);

           fbo2.draw(0,0);
           shader.end();
           fbo.end();

       }

    }
    time = ofGetElapsedTimeMillis()-time;
    ofLogError()<<" update time "<<time;




}

void ofApp::runShader(){


}

//--------------------------------------------------------------
void ofApp::draw(){
    uint64_t time = ofGetElapsedTimeMillis();

    fbo2.draw(0,0);

    std::stringstream strm;
    strm << "FPS: " << ofGetFrameRate()<<endl;
    ofDrawBitmapString(strm.str(),20, ofGetHeight()-20);

    time = ofGetElapsedTimeMillis()-time;
    ofLogError()<<"draw time "<<time;


}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key){
    case '2':
        select = MIN((select+1),8);
        break;
    case '1':
        select = MAX((select-1),0);
        break;
    case 'a':
        x_drift -=1;
        break;
    case 'd':
        x_drift +=1;
        break;
    case 's':
        y_drift +=1;
        num_iterations-=10;

        break;
    case 'w':
        y_drift -=1;
        num_iterations+=10;

        break;
    case 'q':
        bit_offset -=1;
        break;
    case 'e':
        bit_offset +=1;
        break;

    }
    ofLogError()<<"Sort index: " <<select;
    ofLogError()<<"NumIterations: " <<num_iterations;


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




void readFromPixels(u_char * pixels,  int * offsets, int corner, u_char ** pixelPointerArray, int numPixels ){
    for (int index =0; index < numPixels; index++){
        int off =corner + offsets[index];
        u_char pixelValue = pixels[off];
        pixelPointerArray[index]= &pixels[off];
        u_char t = *pixelPointerArray[index];
        int y =0;
    }
}


bool sortAscending(valueOfInterest i,valueOfInterest j)
{
    return (j.hue > i.hue);
}

void ofApp::doMedian(){


    uint64_t time = ofGetElapsedTimeMillis();

    int corner_pixel_index =0;
    for(int y = 0; y < imageHeight; y += 1) {
        for(int x = 0; x < imageWidth; x += 1) {
            int realPixelIndex = corner_pixel_index*3;
            hueValues[corner_pixel_index]=toHue(&(lastFrame->getData()[realPixelIndex]));
            corner_pixel_index++;
        }
    }

    uint64_t calc = ofGetElapsedTimeMillis() -time;
    ofLogError()<<"hue calc time"<< calc<<endl;

    time = ofGetElapsedTimeMillis();


    corner_pixel_index =0;
    for(int y = 0; y < imageHeight-2; y += 1) {
        for(int x = 0; x < imageWidth-2; x += 1) {

            int currentPixelIndex =(corner_pixel_index +1+x_drift +imageWidth*(1+y_drift)) *3;
            if (currentPixelIndex <0 || currentPixelIndex >imageHeight*imageWidth*3) break;

            readFromPixels(lastFrame->getData(), offsets, corner_pixel_index*3, pixelPointerArray, numPixels);
            u_char hue;
            int index = 0;
            for (int w_y = 0; w_y<windowHeight; w_y++){
                for (int w_x = 0; w_x<windowWidth; w_x++){
                   int pixelIndex = corner_pixel_index +w_x +w_y*imageWidth;
                   sortVector[index].hue = hueValues[pixelIndex];
                   sortVector[index].pixelPointer= pixelPointerArray[index];
                   index++;
                }
            }
            //ofSort()
            ofSort(sortVector, sortAscending);
            //sort(sortVector);
            currentPixelIndex+=bit_offset;
            currentFrame->getData()[currentPixelIndex] = sortVector[select].pixelPointer[0];
            currentFrame->getData()[currentPixelIndex+1] = sortVector[select].pixelPointer[1];
            currentFrame->getData()[currentPixelIndex+2] =sortVector[select].pixelPointer[2];;

            corner_pixel_index++;

        }
    }
    calc = ofGetElapsedTimeMillis() -time;
    ofLogError()<<"sort calc time"<< calc<<endl;
    lastFrame->swap(*currentFrame);

}
