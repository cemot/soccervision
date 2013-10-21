#include "DebugRenderer.h"
#include "CameraTranslator.h"
#include "Canvas.h"
#include "Maths.h"
#include "Vision.h"
#include "Util.h"

void DebugRenderer::renderFPS(unsigned char* image, int fps, int width, int height) {
	Canvas canvas = Canvas();

	canvas.data = image;
	canvas.width = width;
	canvas.height = height;

	canvas.drawText(20, 20, "FPS: " + Util::toString(fps));
}

void DebugRenderer::renderBlobs(unsigned char* image, Blobber* blobber, int width, int height) {
	Canvas canvas = Canvas();

	canvas.data = image;
	canvas.width = width;
	canvas.height = height;

	for (int i = 0; i < blobber->getColorCount(); i++) {
		Blobber::Color* color = blobber->getColor(i);

		if (color == NULL) {
			continue;
		}

		if (
			strcmp(color->name, "ball") != 0
			&& strcmp(color->name, "yellow-goal") != 0
			&& strcmp(color->name, "blue-goal") != 0
		) {
			continue;
		}

		Blobber::Blob* blob = blobber->getBlobs(color->name);

		while (blob != NULL) {
			canvas.drawBoxCentered(
				(int)blob->centerX, (int)blob->centerY,
				blob->x2 - blob->x1, blob->y2 - blob->y1,
				color->color.red, color->color.green, color->color.blue
			);

			blob = blob->next;
		}
	}
}

void DebugRenderer::renderBalls(unsigned char* image, const ObjectList& balls, int width, int height) {
	Canvas canvas = Canvas();

	canvas.data = image;
	canvas.width = width;
	canvas.height = height;

	Object* ball = NULL;
    char buf[256];
	int correctedX, correctedY;

    for (ObjectListItc it = balls.begin(); it != balls.end(); it++) {
        ball = *it;

        canvas.drawBoxCentered(ball->x, ball->y, ball->width, ball->height);
		//canvas.drawLine(ball->x - ball->width / 2, ball->y - ball->height / 2, ball->x + ball->width / 2, ball->y + ball->height / 2);
        //canvas.drawLine(ball->x - ball->width / 2, ball->y + ball->height / 2, ball->x + ball->width / 2, ball->y - ball->height / 2);

		sprintf(buf, "%.2fm  %.1f deg", ball->distance, Math::radToDeg(ball->angle));
        canvas.drawText(ball->x - ball->width / 2 + 2, ball->y - ball->height / 2 - 22, buf);

		correctedX = ball->x;
		correctedY = ball->y + ball->height / 2;

		//Util::correctCameraPoint(correctedX, correctedY);

		sprintf(buf, "%d x %d", correctedX, correctedY);
        canvas.drawText(ball->x - ball->width / 2 + 2, ball->y - ball->height / 2 - 12, buf);

        //int boxArea = ball->width * ball->height;

		/*if (boxArea == 0) {
			continue;
		}

        int density = ball->area * 100 / boxArea;

        sprintf(buf, "%d - %d%%", ball->area, density);
        canvas.drawText(ball->x - ball->width / 2 + 2, ball->y - ball->height / 2 - 9, buf);*/
    }

	// TEMP - draw centerline
	//canvas.drawLine(canvas.width / 2, 0, canvas.width / 2, canvas.height);
	//canvas.fillCircleCentered(Config::cameraWidth / 2, Config::cameraHeight / 2, 100, 0, 0, 255);

    /*Blobber::Blob* blob = blobber->getBlobs("ball");

    while (blob != NULL) {
        image->drawBoxCentered(blob->centerX, blob->centerY, blob->x2 - blob->x1, blob->y2 - blob->y1);

        blob = blob->next;
    }*/
}

void DebugRenderer::renderGoals(unsigned char* image, const ObjectList& goals, int width, int height) {
	Canvas canvas = Canvas();

	canvas.data = image;
	canvas.width = width;
	canvas.height = height;

	Object* goal = NULL;
    char buf[256];
	int r, g, b;

    for (ObjectListItc it = goals.begin(); it != goals.end(); it++) {
        goal = *it;

		if (goal->type == Side::YELLOW) {
			r = 200;
			g = 200;
			b = 0;
		} else {
			r = 0;
			g = 0;
			b = 200;
		}

        canvas.drawBoxCentered(goal->x, goal->y, goal->width, goal->height, r, g, b);
		//canvas.drawLine(goal->x - goal->width / 2, goal->y - goal->height / 2, goal->x + goal->width / 2, goal->y + goal->height / 2, r, g, b);
        //canvas.drawLine(goal->x - goal->width / 2, goal->y + goal->height / 2, goal->x + goal->width / 2, goal->y - goal->height / 2, r, g, b);

        sprintf(buf, "%.2fm %.1f deg", goal->distance, Math::radToDeg(goal->angle));
        canvas.drawText(goal->x - goal->width / 2 + 2, goal->y + goal->height / 2 + 2, buf, r, g, b);

		sprintf(buf, "%d x %d, %d", goal->x, goal->y + goal->height / 2, goal->area);
        canvas.drawText(goal->x - goal->width / 2 + 2, goal->y + goal->height / 2 + 12, buf, r, g, b);

        /*int boxArea = goal->width * goal->height;

		if (boxArea == 0) {
			continue;
		}

        int density = goal->area * 100 / boxArea;

        sprintf(buf, "%d - %d%%", goal->area, density);
        canvas.drawText(goal->x - goal->width / 2 + 2, goal->y - goal->height / 2 - 9, buf);*/
    }
}

void DebugRenderer::renderBrush(unsigned char* image, int x, int y, int radius, bool active, int width, int height) {
	Canvas canvas = Canvas();

	canvas.data = image;
	canvas.width = width;
	canvas.height = height;

	canvas.drawCircle(x, y, radius, active ? 255 : 0, 0, active ? 0 : 255);
}

void DebugRenderer::renderObstructions(unsigned char* image, Obstruction obstruction, int width, int height) {
	Canvas canvas = Canvas();

	canvas.data = image;
	canvas.width = width;
	canvas.height = height;

	if (obstruction == Obstruction::BOTH || obstruction == Obstruction::LEFT) {
		canvas.fillBox(width / 2 - 20, height - 80, 20, 40, 200, 0, 0);
	} else {
		canvas.fillBox(width / 2 - 20, height - 80, 20, 40, 0, 200, 0);
	}

	if (obstruction == Obstruction::BOTH || obstruction == Obstruction::RIGHT) {
		canvas.fillBox(width / 2, height - 80, 20, 40, 200, 0, 0);
	} else {
		canvas.fillBox(width / 2, height - 80, 20, 40, 0, 200, 0);
	}
}

void DebugRenderer::renderObjectHighlight(unsigned char* image, Object* object, int red, int green, int blue, int width, int height) {
	Canvas canvas = Canvas();

	canvas.data = image;
	canvas.width = width;
	canvas.height = height;

	canvas.drawBoxCentered(object->x, object->y, object->width + 4, object->height + 4, red, green, blue);
	canvas.drawLine(object->x - object->width / 2, object->y - object->height / 2, object->x + object->width / 2, object->y + object->height / 2, red, green, blue);
    canvas.drawLine(object->x - object->width / 2, object->y + object->height / 2, object->x + object->width / 2, object->y - object->height / 2, red, green, blue);
}

void DebugRenderer::renderGrid(unsigned char* image, Vision* vision, int width, int height) {
	Canvas canvas = Canvas();

	canvas.data = image;
	canvas.width = width;
	canvas.height = height;

	float minDistanceY = 0.0f;
	float maxDistanceY = 4.0f;
	float stepX = 0.25f;
	float stepY = 0.25f;
	float minDistanceX = -4.0f;
	float maxDistanceX = 4.0f;
	float distanceX = 0.0f, distanceY = 0.0f;
	int counter = 0;
	int lastTextY = -1;
	Math::Point screenCoords;
	CameraTranslator::CameraPosition pos;
	CameraTranslator::CameraPosition distorted;
	CameraTranslator::CameraPosition undistorted;

	//for (distanceX = minDistanceX; distanceX <= maxDistanceX; distanceX += stepX) {
		for (distanceY = minDistanceY; distanceY <= maxDistanceY; distanceY += stepY) {
			pos = vision->getPixelAt(distanceX, distanceY);

			for (int x = 0; x < Config::cameraWidth; x += 3) {
				distorted = vision->getCameraTranslator()->distort(x, pos.y);
				//undistorted = vision->getCameraTranslator()->undistort(distorted.x, distorted.y);

				//canvas.setPixelAt(x, y, 0, 0, 128);
				canvas.setPixelAt(distorted.x, distorted.y, 0, 0, 128);
				//canvas.setPixelAt(undistorted.x, undistorted.y, 128, 0, 0);
			}

			/*for (int y = 0; y < Config::cameraHeight; y += 3) {
				distorted = vision->getCameraTranslator()->distort(pos.x, y);
				//undistorted = vision->getCameraTranslator()->undistort(distorted.x, distorted.y);

				//canvas.setPixelAt(x, y, 0, 0, 128);
				canvas.setPixelAt(distorted.x, distorted.y, 0, 0, 128);
				//canvas.setPixelAt(undistorted.x, undistorted.y, 128, 0, 0);
			}*/

			//px = 10 + (counter % 10) * 30;
			int x = Config::cameraWidth / 2 - 15;

			distorted = vision->getCameraTranslator()->distort(x, pos.y + 1);

			if (lastTextY == -1 || lastTextY - distorted.y >= 10) {
				canvas.drawText(distorted.x, distorted.y, Util::toString(distanceY), 128, 128, 128);

				lastTextY = distorted.y;
			}

			/*for (distanceX = minDistanceX; distanceX < maxDistanceX; distanceX += stepX) {
				for (int y = 0; y < Config::cameraHeight; y++) {
					screenCoords = vision->getScreenCoords(vision->getDir(), distanceX, distanceY);
				}
			}*/

			counter++;
		}
	//}

	

	
}