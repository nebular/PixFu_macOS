//
// Created by rodo on 2020-02-29.
//

#pragma once

#include "glm.hpp"

class WalkableLine
{
	glm::vec2 start;
	glm::vec2 end;

	float cosAngle, sinAngle, angle, length;

private:
	void calculate() {
		angle = atan2(end.y-start.y, end.x-start.x);
		sinAngle = sinf(angle);
		cosAngle = cosf(angle);
		length = sqrt((start.x-end.x)*(start.x-end.x)+(start.y-end.y)*(start.y-end.y));
	}

public:
	WalkableLine(glm::vec2 s, glm::vec2 e) {
		start=s;
		end=e;
		calculate();

	}

	bool isInTrack(float position) {
		return position>0 && position < length;
	}

	bool reset(glm::vec2 startPoint) {
		if (start.x != startPoint.x && start.y != startPoint.y) {
			start = startPoint;
			calculate();
			return true;
		} else return false;
	}
	float getAngle() {
		return angle;
	}

	float getLength() {
		return length;
	}

	glm::vec2 getPoint(float lenstep) {
		return { lenstep*cosAngle + start.x, lenstep*sinAngle+start.y };
	}
	void DrawSelf(rgl::Canvas2D *gfx, rgl::Pixel col = 0x000F) {
		gfx->drawLine(start.x, start.y, end.x, end.y, col);
	}


};