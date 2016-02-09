#include "ofApp.h"

using namespace ofxCvGui;

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowTitle("Server");
	ofSetWindowShape(512, 1024);
	ofSetWindowPosition(256, 256);

	gui.init();
	this->widgetsPanel = gui.addWidgets();
	
	{
		this->listener.port.set("Port", 4444);
		this->listener.iterationsPerFrame.set("Iterations per frame", 100);
	}

	this->rebuildGui();

	this->serverThread = thread([this]() {
		while (true) {
			this->lockServer.lock();
			{
				if (this->server) {
					size_t sizeReceived = 0;
					ofxProfiler::Activity activity;
					activity.begin();

					bool contiguous = true;
					auto iterations = this->listener.iterationsPerFrame.get();
					for (int i = 0; i < iterations; i++) {
						auto dataGram = this->server->receive(2e5);
						if (dataGram) {
							const auto & message = dataGram->getMessage();
							if (message.length() > 16) {
								auto messageIntPtr = (int*)& message[0];

								bool valid = true;
								for (int i = 0; i < message.size() / 16; i += 16) {
									if (messageIntPtr[i + 2] != i) {
										valid = false;
										break;
									}
								}
								this->latestDataGram.valid = valid;

								contiguous &= messageIntPtr[1] - this->latestDataGram.packetIndex == 1;
								this->latestDataGram.frameIndex = messageIntPtr[0];
								this->latestDataGram.packetIndex = messageIntPtr[1];
								this->latestDataGram.size = message.length();

								sizeReceived += this->latestDataGram.size;
								this->history.lock.lock();
								{
									this->history.frameIndex.insert(this->latestDataGram.frameIndex);
								}
								this->history.lock.unlock();
							}
						}
					}

					this->history.lock.lock();
					{
						this->latestDataGram.contiguous = contiguous;
						if (this->history.frameIndex.size() > 2) {
							auto firstIndex = this->history.frameIndex.begin();

							auto lastIndex = this->history.frameIndex.end();
							lastIndex--;

							auto intendedRange = *lastIndex - *firstIndex;
							auto actualRange = this->history.frameIndex.size();

							this->history.successRate = 100.0f * (float)actualRange / (float)intendedRange;
						}
						else {
							this->history.successRate = 0.0f;
						}
					}
					this->history.lock.unlock();
					activity.end();
					this->history.dataRate = (float) ((double) sizeReceived / activity.getDuration()) / 1e6;
				}
			}
			this->lockServer.unlock();

			//this_thread::sleep_for(1us);
		}
	});
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

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

//--------------------------------------------------------------
void ofApp::rebuildGui() {
	this->widgetsPanel->clear();

	this->widgetsPanel->add(Widgets::makeFps());

	this->widgetsPanel->add(Widgets::LiveValueHistory::make("Megabytes per second", [this]() {
		return this->history.dataRate;
	}));

	this->widgetsPanel->add(Widgets::Title::make("Listener"));
	{
		this->widgetsPanel->add(Widgets::EditableValue<int>::make(this->listener.port));
		
		this->widgetsPanel->add(Widgets::EditableValue<int>::make(this->listener.iterationsPerFrame));

		auto connectToggle = Widgets::Toggle::make("Connect", [this]() {
			if (this->server) {
				return true;
			}
			else {
				return false;
			}
		}, [this](bool connect) {
			if (connect) {
				this->connect();
			}
			else {
				this->disconnect();
			}
		});
		this->widgetsPanel->add(connectToggle);
	}

	if (this->server) {
		this->widgetsPanel->add(Widgets::Title::make("Latest DataGram"));
		{
			this->widgetsPanel->add(Widgets::Indicator::make("Valid", [this]() {
				if (this->latestDataGram.valid) {
					return Widgets::Indicator::Good;
				}
				else {
					return Widgets::Indicator::Clear;
				}
			}));
			this->widgetsPanel->add(Widgets::Indicator::make("Contiguous", [this]() {
				if (this->latestDataGram.contiguous) {
					return Widgets::Indicator::Good;
				}
				else {
					return Widgets::Indicator::Clear;
				}
			}));
			this->widgetsPanel->add(Widgets::LiveValue<int>::make("Size", [this]() {
				return this->latestDataGram.size;
			}));
			this->widgetsPanel->add(Widgets::LiveValue<int>::make("Last frame index", [this]() {
				return this->latestDataGram.frameIndex;
			}));
			this->widgetsPanel->add(Widgets::LiveValue<int>::make("Last packet index", [this]() {
				return this->latestDataGram.packetIndex;
			}));
			this->widgetsPanel->add(Widgets::LiveValueHistory::make("Success rate [%]", [this]() {
				this->history.lock.lock();
				auto successRate = this->history.successRate;
				this->history.lock.unlock();
				return successRate;
			}));
			this->widgetsPanel->add(Widgets::Button::make("Clear history", [this]() {
				this->history.lock.lock();
				this->history.frameIndex.clear();
				this->history.lock.unlock();
			}));
		}
	}
}

//--------------------------------------------------------------
void ofApp::connect() {
	this->disconnect();

	this->lockServer.lock();
	{
		this->server = make_shared<ofxAsio::UDP::Server>(this->listener.port);
		this->history.lock.lock();
		{
			this->history.frameIndex.clear();
		}
		this->history.lock.unlock();
	}
	this->lockServer.unlock();

	this->rebuildGui();
}

//--------------------------------------------------------------
void ofApp::disconnect() {
	this->lockServer.lock();
	{
		this->server.reset();
	}
	this->lockServer.unlock();

	this->rebuildGui();
}