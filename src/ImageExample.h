#pragma once

#include "ofMain.h"
#include "ofxTuioClient.h"
// for openGL
#include "ofGLRenderer.h"

//#define WINDOW_WIDTH 1024
//#define WINDOW_HEIGHT 768

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 800

class ImageExample : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

	// added to Example
		ofxTuioClient myTuio;
    

		void touchDown(ofTouchEventArgs & touch);
		void touchMoved(ofTouchEventArgs & touch);
		void touchUp(ofTouchEventArgs & touch);
    
    void checkCollision(float m_ImgPositionX, float m_ImgPositionY);

private:
	ofGLRenderer m_OpenGLRenderer;
    
    // 이미지들
	ofImage m_player1;
	ofImage m_player2;
    ofImage m_BgImage;
    ofImage m_CautionImage;
    
    // 글씨
    ofTrueTypeFont		font; // for displaying messages on window

    // 사운드
    ofSoundPlayer bgm;
    ofSoundPlayer pop;
    ofSoundPlayer excellent;
    
    // 점수
    string score1;
    string score2;
    float  scoreValue1;
    float  scoreValue2;
    
    // 득점 효과
    bool goalEffect1;
    bool goalEffect2;
    
    // 하키채 위치,크기,각도
	float m_ImgPositionX1;
    float m_ImgPositionY1;
    float m_ImgPositionX2;
    float m_ImgPositionY2;
    
	float m_ImgWidth;
	float m_ImgHeight;
	float m_ImgRotateAngle;
    
    
    // ball
    float b_xPosition;
    float b_yPosition;
    float b_radius;
    float b_xSpeed;
    float b_ySpeed;
    int b_xDirection;
    int b_yDirection;
    
    // 충돌
    float spring;
    
    // 경고
    bool caution;
};
