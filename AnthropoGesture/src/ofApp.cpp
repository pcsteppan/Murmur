#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	resr = 3;
	resh = 1;
	resc = 1;
	network = Network();
	ofSetConeResolution(resr, resh, resc);
	ofSetBackgroundAuto(false);
	ofBackground(255,255,255);
	ofSetFrameRate(30);

	// RECORDING SETUP
	// m_Grabber.setup(3840, 2160);
	m_Fbo.allocate(3840, 2160, GL_RGBA, 2);
	m_Recorder.setup(true, false, glm::vec2(m_Fbo.getWidth(), m_Fbo.getHeight()));
	m_Recorder.setOverWrite(true);
	//m_Recorder.setVideoCodec("mjpeg");
	m_Recorder.setFps(30);
	//m_Recorder.setBitRate(39062);

	// EASYCAM SETUP
	cam.setTarget(glm::vec3(0,0,0));
	cam.setFov(110);
	cam.setFarClip(cam.getFarClip() - 1000);
	//circleResolution.addListener(this, &ofApp::circleResolutionChanged);
	//ringButton.addListener(this, &ofApp::ringButtonPressed);

	gui.setup(); // most of the time you don't need a name
	gui.add(radiusAlign.setup("r:align", 200, 0, 500));
	gui.add(radiusCohere.setup("r:cohere", 200, 0, 500));
	gui.add(radiusSeparate.setup("r:separate", 200, 0, 500));

	gui.add(weightAlign.setup("w:align", 0.5, 0, 1));
	gui.add(weightCohere.setup("w:cohere", 0.5, 0, 1));
	gui.add(weightSeparate.setup("w:separate", 0.5, 0, 1));
	gui.add(weightCenterPull.setup("w:center", 0, 0, 1));
	//gui.add(filled.setup("fill", true));
	//gui.add(radius.setup("radius", 140, 10, 300));
	//gui.add(center.setup("center", { ofGetWidth()*.5, ofGetHeight()*.5 }, { 0, 0 }, { ofGetWidth(), ofGetHeight() }));
	//gui.add(color.setup("color", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
	//gui.add(circleResolution.setup("circle res", 5, 3, 90));
	//gui.add(twoCircles.setup("two circles"));
	//gui.add(ringButton.setup("ring"));
	//gui.add(screenSize.setup("screen size", ofToString(ofGetWidth()) + "x" + ofToString(ofGetHeight())));

	bHide = bDrawOctree = bClearImage = false;
	

	//ring.load("ring.wav");
}

//--------------------------------------------------------------
/*
void ofApp::exit() {
	ringButton.removeListener(this, &ofApp::ringButtonPressed);
}
*/

//--------------------------------------------------------------
void ofApp::update(){
	network.updateFactors(weightAlign, weightCohere, weightSeparate, weightCenterPull, radiusAlign, radiusCohere, radiusSeparate);
	network.difference();
	printf("framerate: %f\n", ofGetFrameRate());

	// RECORDING UPDATE
	

}

//--------------------------------------------------------------
void ofApp::draw(){
	m_Fbo.begin();
	if(bClearImage) ofBackground(255, 255, 255);
	cam.begin();

	network.represent(bDrawOctree);

	cam.end();
	m_Fbo.end();

	if (m_Recorder.isRecording()) {
		m_Reader.readToPixels(m_Fbo, m_Pix);
		if (m_Pix.getWidth() > 0 && m_Pix.getHeight() > 0) {
			m_Recorder.addFrame(m_Pix);
		}
	}

	m_Fbo.draw(0,0,1920,1080);

	if (!bHide) {
		gui.draw();
		ofDrawBitmapStringHighlight(std::to_string(m_Recorder.getRecordedDuration()), 40, 45);
		ofDrawBitmapStringHighlight("FPS: " + std::to_string(ofGetFrameRate()), 10, 16);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	m_Fbo.begin();
	ofClear(255, 255, 255);
	m_Fbo.end();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == 'h') {
		bHide = !bHide;
	}
	else if (key == 'b') {
		bClearImage = !bClearImage;
	}
	else if (key == 'o') {
		bDrawOctree = !bDrawOctree;
	}
	else if (key == ' ')
	{
		if (m_Recorder.isRecording()) {
			m_Recorder.stop();
		}
		else {
			m_Recorder.setOutputPath("C:/Vids/" + ofGetTimestampString() + ".avi");
			//m_Recorder.setOutputPath("C:/Users/Andrew/Documents/Patrick/" + ofGetTimestampString() + ".avi");
			m_Recorder.startCustomRecord();
		}
	}
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
