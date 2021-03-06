#include "ofApp.h"

ofEasyCam cam;
ofxColorSlider colorGround;
ofxColorSlider nearColorBoid;
ofxColorSlider farColorBoid;
ofxColorSlider colorGrid;
ofImage palette;
ofPixels palettePixels;
ofxFloatSlider maxForce;
ofxFloatSlider maxVelocity;
ofxFloatSlider weightCenterPullX;
ofxFloatSlider weightCenterPullY;
ofxFloatSlider weightCenterPullZ;
ofxFloatSlider actorSize;
ofxFloatSlider weightPullDirectionalX;
ofxFloatSlider weightPullDirectionalY;
ofxFloatSlider weightPullDirectionalZ;
ofxToggle colorSplit;

//--------------------------------------------------------------
void ofApp::setup(){
	resr = 6;
	resh = 1;
	resc = 1;
	//network = Network();
	ofSetConeResolution(resr, resh, resc);
	ofSetBackgroundAuto(false);
	ofBackground(255,255,255);
	ofSetFrameRate(24);

	// PALETTE SETUP
	palette.load("./images/palette1.png");
	palettePixels = palette.getPixels();

	network = Network();

	// RECORDING SETUP
	// m_Grabber.setup(3840, 2160);
	// ffmpeg recorder fbo can't use anti-aliasing it seems?
	// which would be the fourth argument after GL_RGB
	// like 2, which would make the lines smooth instead of pixellated
	//m_Fbo.allocate(1920, 1080, GL_RGBA);
	m_Fbo.allocate(1920, 1080, GL_RGBA);
	m_Recorder.setup(true, false, glm::vec2(m_Fbo.getWidth(), m_Fbo.getHeight()));
	m_Recorder.setOverWrite(true);
	//m_Recorder.setVideoCodec("mjpeg");
	m_Recorder.setFps(24);
	m_Recorder.setBitRate(1024*40);

	// EASYCAM SETUP
	cam.setTarget(glm::vec3(0,0,0));
	cam.setFov(120);
	cam.setFarClip(cam.getFarClip() - 1000);
	//circleResolution.addListener(this, &ofApp::circleResolutionChanged);
	//ringButton.addListener(this, &ofApp::ringButtonPressed);

	gui.setup(); // most of the time you don't need a name

	//gui.add(cameraAngleDelta.setup("cam-angle diff", { 0, 0, 0 }, { 0, 0, 0 }, { TWO_PI, TWO_PI, TWO_PI }));
	gui.add(floatRadius.setup("camera radius", 1000, 200, 2500));


	gui.add(radiusAlign.setup("r:align", 300, 0, 500));
	gui.add(radiusCohere.setup("r:cohere", 400, 0, 500));
	gui.add(radiusSeparate.setup("r:separate", 80, 0, 500));

	gui.add(maxForce.setup("max force", 0.32f, 0.0f, 0.6f));
	gui.add(maxVelocity.setup("max velocity", 24.0f, 0.0f, 100.0f));
	gui.add(actorSize.setup("actor size", 20.0f, 0.0f, 100.0f));

	gui.add(weightAlign.setup("w:align", 0.6, 0, 1));
	gui.add(weightCohere.setup("w:cohere", 0.75, 0, 1));
	gui.add(weightSeparate.setup("w:separate", 1, 0, 1));
	gui.add(weightCenterPull.setup("w:center", 0.3, 0, 1));

	gui.add(weightCenterPullX.setup("w:center.x", 1, 0, 1));
	gui.add(weightCenterPullY.setup("w:center.y", 1, 0, 1));
	gui.add(weightCenterPullZ.setup("w:center.z", 1, 0, 1));

	gui.add(weightPullDirectionalX.setup("w:dir.x", 0, -1, 1));
	gui.add(weightPullDirectionalY.setup("w:dir.y", 0, -1, 1));
	gui.add(weightPullDirectionalZ.setup("w:dir.z", 0, -1, 1));

	//gui.add(filled.setup("fill", true));
	//gui.add(radius.setup("radius", 140, 10, 300));
	//gui.add(center.setup("center", { ofGetWidth()*.5, ofGetHeight()*.5 }, { 0, 0 }, { ofGetWidth(), ofGetHeight() }));
	gui.add(colorGrid.setup("grid color", ofColor(0,0,0,5), ofColor(0, 0), ofColor(255, 255)));
	gui.add(farColorBoid.setup("far boid color", ofColor(255, 255, 255, 40), ofColor(0, 0), ofColor(255, 255)));
	gui.add(nearColorBoid.setup("near boid color", ofColor(255, 255, 255, 1), ofColor(0, 0), ofColor(255, 255)));
	gui.add(colorSplit.setup("split", false));
	gui.add(colorGround.setup("ground color", ofColor(0, 0, 0, 255), ofColor(0, 0), ofColor(255, 255)));
	
	//gui.add(circleResolution.setup("circle res", 5, 3, 90));
	//gui.add(twoCircles.setup("two circles"));
	//gui.add(ringButton.setup("ring"));
	//gui.add(screenSize.setup("screen size", ofToString(ofGetWidth()) + "x" + ofToString(ofGetHeight())));

	bHide = bDrawOctree = false;
	bClearImage = true;
	//ring.load("ring.wav");
	recordingFrameRate = 24;
	now = 0;
}

//--------------------------------------------------------------
/*
void ofApp::exit() {
	ringButton.removeListener(this, &ofApp::ringButtonPressed);
}
*/

//--------------------------------------------------------------
void ofApp::update(){
	now += 1. / (float)recordingFrameRate;

	network.updateFactors(weightAlign, weightCohere, weightSeparate, weightCenterPull, radiusAlign, radiusCohere, radiusSeparate);
	network.difference();
	printf("framerate: %f\n", ofGetFrameRate());

	// noise for latitude and longitude
	
	
	float longitude = ofMap(ofSignedNoise(now/100.0f), -1, 1, -360.0f*4, 360.0f*4);
	// ofGetElapsedTimef() * 14
	float latitude = sin(ofMap((int)(now*1000) % 120000, 0, 111119, 0.0f, 2.0f*PI))*45.0f;
	cam.orbit(longitude, latitude, floatRadius, ofVec3f(0, 0, 0));
	// long (degrees), lat, radius, center pt
	cam.lookAt(ofVec3f(0, 0, 0));
	// RECORDING UPDATE
	//float latitude = cameraAngleDelta.getFloatSlider("x");
	//latitude += atan2(cam.getPosition().x, cam.getPosition().y);
	//float longitude = cameraAngleDelta.getFloatSlider("y");
	//longitude += atan2(cam.getPosition().y, cam.getPosition().z);
	//float z = cameraAngleDelta.getFloatSlider("z");
	//glm::vec3 orientation = cam.getOrientationEulerRad();
	//cam.orbitRad(0.02, longitude, 400, glm::vec3(0,0,0));
	//cam.orbitRad((float)((orientation.x*2) % 6), 0.0, 200, glm::vec3(0, 0, 0));
	//cam.rotateAroundRad(, glm::vec3(0,1,0), glm::vec3(0,0,0));
	//cam.rotateAroundRad(cameraAngleDelta.getFloatSlider("z"), glm::vec3(0,0,1), glm::vec3(0,0,0));
}

//--------------------------------------------------------------
void ofApp::draw(){
	m_Fbo.begin();
	
	if (bClearImage) {
		ofClear(colorGround);
		//ofBackground(255, 255, 255);
	}
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
		ofDrawBitmapStringHighlight(std::to_string(m_Recorder.getRecordedDuration()), ofGetWidth()-140, 45);
		ofDrawBitmapStringHighlight("FPS: " + std::to_string(ofGetFrameRate()), ofGetWidth()-100, 16);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
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
	else if (key == 'c')
	{
		colorSplit = !colorSplit;
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
