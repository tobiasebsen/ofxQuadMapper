//
//  Quadrilateral.h
//
//  Created by Tobias Ebsen on 13/07/15.
//
//

#pragma once

#include "ofMain.h"

class ofxQuadMapper {
public:
	ofxQuadMapper();
	ofxQuadMapper(ofVec3f * corners) {
        setCorners(corners);
    }
    
    void setCorners(const ofVec3f * corners) {
        setCorner(0, corners[0]);
        setCorner(1, corners[1]);
        setCorner(2, corners[2]);
        setCorner(3, corners[3]);
    }
    void setCorner(int i, const ofVec2f & corner) {
        setCorner(i, corner.x, corner.y);
    }
    void setCorner(int i, const ofVec3f & corner) {
        setCorner(i, corner.x, corner.y);
    }
    void setCorner(int i, float x, float y);
    
    ofVec2f getCorner(int i) {
        return corners[i];
    }

	ofVec2f * getCorners() {
		return corners;
	}

    ofVec2f getCenter() {
        return (corners[0]+corners[1]+corners[2]+corners[3])/4;
    }

	bool isSorted() {
        ofVec2f c = getCenter();
        return corners[0].x < c.x && corners[0].y < c.y &&
            corners[1].x > c.x && corners[1].y < c.y &&
            corners[2].x > c.x && corners[2].y > c.y &&
            corners[3].x < c.x && corners[3].y > c.y;
    }
    
    ofVec2f map(const ofVec3f & p) {
        return map(p.x, p.y);
    }
    ofVec2f map(const ofVec2f & p) {
        return map(p.x, p.y);
    }
    ofVec2f map(float x, float y);
    
    ofVec2f unmap(const ofVec2f & p) {
        return unmap(p.x, p.y);
    }
    ofVec2f unmap(float x, float y);

    bool inside(const ofVec3f & point) {
        return inside(ofVec2f(point));
    }
    bool inside(const ofVec2f & point) {
        ofVec2f m = map(point.x, point.y);
        return m.x >= 0. && m.x <= 1. && m.y >= 0. && m.y <= 1.;
    }
    
    void draw();

	void save(string filepath = "quad.xml");
	bool load(string filepath = "quad.xml");

private:
    ofVec2f corners[4];
};
