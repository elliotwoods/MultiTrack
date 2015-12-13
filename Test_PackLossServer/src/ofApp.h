#pragma once

#include "ofMain.h"
#include "ofxCvGui.h"
#include "ofxAsio.h"

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
		
		void rebuildGui();
		void connect();
		void disconnect();

		ofxCvGui::Builder gui;
		shared_ptr<ofxCvGui::Panels::Scroll> widgetsPanel;

		shared_ptr<ofxAsio::UDP::Server> server;

		struct {
			ofParameter<int> port;
			ofParameter<int> iterationsPerFrame;
		} listener;

		struct {
			bool valid = false;
			bool contiguous = false;
			int size;
			int frameIndex;
		} latestDataGram;

		struct {
			set<int> frameIndex;
			float successRate;
		} history;
};
