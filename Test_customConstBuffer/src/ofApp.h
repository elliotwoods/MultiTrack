#pragma once

#include "ofMain.h"
#include "ofxCvGui.h"
#include "ofxAsio.h"
#include "ofxProfiler.h"


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
		shared_ptr<ofxCvGui::Panels::Widgets> widgetsPanel;

		shared_ptr<ofxAsio::UDP::Client> client;
		mutex clientMutex;

		struct {
			ofParameter<string> ip;
			ofParameter<int> port;
		} sender;
};
