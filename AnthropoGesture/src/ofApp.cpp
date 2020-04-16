#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	resr = 3;
	resh = 1;
	resc = 1;
	network = Network();
	ofSetConeResolution(resr, resh, resc);
	cam.setTarget(glm::vec3(ofGetWidth() / 2, ofGetHeight() / 2, 0));
}

//--------------------------------------------------------------
void ofApp::update(){
	network.difference();
	printf("framerate: %f\n", ofGetFrameRate());
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(245, 245, 255, 10);
	cam.begin();
	network.represent();
	cam.end();
	//ofDrawEllipse(mouseX, mouseY, 20, 20);
	//ofSleepMillis(8000);
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