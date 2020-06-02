#pragma once

#include "ofMain.h"
#include "Network.h"
#include "ofVideoGrabber.h"
#include "ofxFFmpegRecorder.h"
#include "ofxFastFboReader.h"
#include "ofxGui.h"

extern ofEasyCam cam;
extern ofxColorSlider nearColorBoid;
extern ofxColorSlider farColorBoid;
extern ofxColorSlider colorGround;
extern ofxColorSlider colorGrid;
extern ofxFloatSlider maxForce;
extern ofxFloatSlider maxVelocity;
extern ofImage palette;
extern ofPixels palettePixels;
extern ofxFloatSlider weightCenterPullX;
extern ofxFloatSlider weightCenterPullY;
extern ofxFloatSlider weightCenterPullZ;
extern ofxFloatSlider actorSize;

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
		
	private:
		Network network;
		
		
		// RECORDING VARS
		// ofVideoGrabber m_Grabber;
		ofxFFmpegRecorder m_Recorder;
		ofxFastFboReader m_Reader;
		ofFbo m_Fbo;
		ofPixels m_Pix;

		int resr, resh, resc;

		//void circleResolutionChanged(int & circleResolution);
		//void ringButtonPressed();

		bool bHide;
		bool bDrawOctree;
		bool bClearImage;

		ofxFloatSlider radiusAlign;
		ofxFloatSlider radiusCohere;
		ofxFloatSlider radiusSeparate;

		ofxFloatSlider weightAlign;
		ofxFloatSlider weightCohere;
		ofxFloatSlider weightSeparate;
		ofxFloatSlider weightCenterPull;
		
		ofxFloatSlider floatRadius;

		ofxVec3Slider cameraAngleDelta;
		
		//ofxVec2Slider center;
		//ofxIntSlider circleResolution;
		//ofxToggle filled;
		//ofxButton twoCircles;
		//ofxButton ringButton;
		//ofxLabel screenSize;

		ofxPanel gui;

		int recordingFrameRate;
		float now;
};
