#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openFrameworks");

	ofBackground(39);
	ofSetRectMode(ofRectMode::OF_RECTMODE_CENTER);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);

	for (int i = 0; i < 1; i++) {

		this->noise_seed_list.push_back(glm::vec2(ofRandom(1000), ofRandom(1000)));
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	int index = 0;
	auto radius = 30;
	ofColor color;
	for (auto& noise_seed : this->noise_seed_list) {

		auto deg = ofGetFrameNum() * 6 + index * 60;
		auto next_deg = deg + 3;

		auto location = glm::vec2(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD));
		auto next = glm::vec2(radius * cos(next_deg * DEG_TO_RAD), radius * sin(next_deg * DEG_TO_RAD));

		auto distance =  next - location;
		distance *= 1;

		for (int i = 0; i < 1; i++) {

			auto future = location + distance * 8;
			auto random_deg = ofRandom(360);
			future += glm::vec2(15 * cos(random_deg * DEG_TO_RAD), 15 * sin(random_deg * DEG_TO_RAD));
			auto future_distance = future - location;

			this->location_list.push_back(location);
			this->velocity_list.push_back(glm::normalize(future_distance) * glm::length(distance));

			color.setHsb(ofMap(deg % 180, 0, 180, 0, 255), 200, 200);
			this->color_list.push_back(color);
		}

		index++;
	}

	for (int i = this->location_list.size() - 1; i > -1; i--) {

		this->location_list[i] += this->velocity_list[i];
		this->velocity_list[i] *= 1.0005;

		if (glm::distance(glm::vec2(), this->location_list[i]) > 360) {

			this->location_list.erase(this->location_list.begin() + i);
			this->velocity_list.erase(this->velocity_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);
		}
	}

}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWindowSize() * 0.5);
	ofRotateY(180);

	int index = 0;
	for (auto& location : this->location_list) {

		auto size = ofMap(glm::length(location), 0, 360, 6, 32);
		auto len = glm::length(location);
		auto alpha = len < 200 ? 255 : ofMap(len, 200, 320, 255, 0);

		ofPushMatrix();
		ofTranslate(location);

		ofFill();
		ofSetColor(this->color_list[index], alpha * 0.75);
		ofDrawCircle(glm::vec2(), size * 0.65);

		ofNoFill();
		ofSetColor(this->color_list[index], alpha);
		ofDrawCircle(glm::vec2(), size * 0.65);
		ofDrawCircle(glm::vec2(), size);

		ofPopMatrix();

		index++;
	}

	/*
	int start = 500;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
	*/
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}