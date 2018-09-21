//
//  Quadrilateral.cpp
//
//  Created by Tobias Ebsen on 13/07/15.
//
//

#include "ofxQuadMapper.h"

ofxQuadMapper::ofxQuadMapper()  {
}

void ofxQuadMapper::setCorner(int i, float x, float y) {
    corners[i].set(x, y);
}

float inline cross(ofVec2f & a, ofVec2f b) {
    return a.x * b.y - a.y * b.x;
}

ofVec2f ofxQuadMapper::map(float x, float y) {

    // Inverse bilinear interpolation
    // https://www.particleincell.com/2012/quad-interpolation/
    // http://www.iquilezles.org/www/articles/ibilinear/ibilinear.htm
    
    ofVec2f e = corners[1] - corners[0];
    ofVec2f f = corners[3] - corners[0];
    ofVec2f g = corners[0] - corners[1] + corners[2] - corners[3];
    ofVec2f h = ofVec2f(x,y) - corners[0];
    
    float a = cross(g, f);
    float b = cross(e, f) + cross(h, g);
    float c = cross(h, e);

    ofVec2f p;
    
    if (a != 0) {
        // quadric equation : 0 = a * x * x + b * x + c
        float w = b * b - 4 * a * c;
        if (w < 0)
            return ofVec2f(-1);
        p.y = (-b + sqrt(w)) / (2 * a);
    }
    else {
        // linear equation
        p.y = - c / b;
    }
    
    float dd = (e.x + g.x * p.y);
    if (dd != 0) {
        p.x = (h.x - f.x * p.y) / (e.x + g.x * p.y);
    }
    else {
        p.x = (h.y - f.y * p.y) / (e.y + g.y * p.y);
    }
    
    return p;
}

ofVec2f ofxQuadMapper::unmap(float x, float y) {
    ofVec2f u1 = corners[0].getInterpolated(corners[1], x);
    ofVec2f u2 = corners[3].getInterpolated(corners[2], x);
    return u1.interpolate(u2, y);
}

void ofxQuadMapper::draw() {
    ofBeginShape();
    ofVertex(corners[0].x, corners[0].y);
    ofVertex(corners[1].x, corners[1].y);
    ofVertex(corners[2].x, corners[2].y);
    ofVertex(corners[3].x, corners[3].y);
    ofVertex(corners[0].x, corners[0].y);
    ofEndShape();
    /*ofDrawBitmapString("1", corners[0]);
    ofDrawBitmapString("2", corners[1]);
    ofDrawBitmapString("3", corners[2]);
    ofDrawBitmapString("4", corners[3]);*/
}

void ofxQuadMapper::save(string filepath) {
	ofXml xml;
	xml.addChild("quad");
	xml.setTo("quad");
	for (int i=0; i<4; i++) {
		xml.addChild("corner");
		xml.setToChild(i);
		xml.addValue("x", corners[i].x);
		xml.addValue("y", corners[i].y);
		xml.setToParent();
	}
	xml.save(filepath);
}

bool ofxQuadMapper::load(string filepath) {
	ofXml xml;
	if (!xml.load(filepath))
		return false;

	if (!xml.setTo("quad"))
		return false;

	int n = MIN(4, xml.getNumChildren());
	for (int i=0; i<n; i++) {
		xml.setToChild(i);
		if (xml.getName() == "corner") {
			int m = xml.getNumChildren();
			for (int j=0; j<m; j++) {
				xml.setToChild(j);
				if (xml.getName() == "x")
					corners[i].x = xml.getFloatValue();
				if (xml.getName() == "y")
					corners[i].y = xml.getFloatValue();
				xml.setToParent();
			}
		}
		xml.setToParent();
	}
	return true;
}