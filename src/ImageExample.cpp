#include "ImageExample.h"

//--------------------------------------------------------------
void ImageExample::setup(){
	ofSetFrameRate(60);
	// Connect
	myTuio.connect(3333);

	ofAddListener(ofEvents().touchDown, this, &ImageExample::touchDown);
	ofAddListener(ofEvents().touchMoved, this, &ImageExample::touchMoved);
	ofAddListener(ofEvents().touchUp, this, &ImageExample::touchUp);

	m_player1.loadImage("hockey1.png");
    m_player2.loadImage("hockey2.png");
    m_BgImage.loadImage("bg.jpg");
    m_CautionImage.loadImage("caution.png");

    // 점수판 글씨 (사이즈가 100)
    font.loadFont("DIN.otf", 100);
    
    // score setup
    scoreValue1 = 0;
    scoreValue2 = 0;
    
    goalEffect1 = false;
    goalEffect2 = false;
    
    bgm.loadSound("bgm.mp3");
    pop.loadSound("pop.mp3");
    excellent.loadSound("excellent.mp3");
    
    bgm.play();
    bgm.setLoop(true);
    
    
	m_OpenGLRenderer = new ofGLRenderer();
    
    
    // 하키채 setup
	m_ImgPositionX1 = 100.0;
	m_ImgPositionY1 = WINDOW_HEIGHT/2+50;
    
    m_ImgPositionX2 = WINDOW_WIDTH-100.0;
    m_ImgPositionY2 = WINDOW_HEIGHT/2+50;
    
	m_ImgWidth = 150.0;
	m_ImgHeight = 150.0;
	m_ImgRotateAngle = 120.0;
    
    
    
    // ball setup
    b_xPosition = ofGetWindowWidth()/2.0;
    b_yPosition = ofGetWindowHeight()/2.0;
    
    b_radius = 30; // 볼의 지름
    
    b_xSpeed = 15.8;
    b_ySpeed = 13.2;
    
    b_xDirection = 1;
    b_yDirection = 1;
    
    ofSetFrameRate(30);
    
    
    spring = 0.05; // 충돌 시 튕기는 정도
    caution = false;
    
}

//--------------------------------------------------------------
void ImageExample::update(){
    
    vector<TuioCursor> cursorList;
    cursorList.clear();
    
    for(TuioCursor *iter : myTuio.client->getTuioCursors())
    {
        cursorList.push_back(iter);
    }

    for (int i=0; i<cursorList.size(); i++){
        int movedPositionX = cursorList[i].getX() * WINDOW_WIDTH;
        int movedPositionY = cursorList[i].getY() * WINDOW_HEIGHT;
        
        if(WINDOW_WIDTH/2 >= movedPositionX ){ // player1의 영역에서의 터치는 빨간 하키채 제어
            m_ImgPositionX1 = movedPositionX;
            m_ImgPositionY1 = movedPositionY;
        }
        
        else if (WINDOW_WIDTH > movedPositionX){ // player2의 영역에서의 터치는 파란 하키채 제어
            m_ImgPositionX2 = movedPositionX;
            m_ImgPositionY2 = movedPositionY;
        }
    }

    
    // 터치수에 따른 이벤트 지정
    int numPoint = cursorList.size();
    
    if ( numPoint >2 ){  // 터치수가 3개가 되면 반칙 혹은 실수로 터치된거라고 간주하여 경고창이 나옴
        caution = true;  // 경고창 보이기
        }
    else {               // 터치수가 0~2개 까지는 2명의 플레이어가 있다고 간주
        caution = false; // 경고창 없애기
    }
    
    
    
    
    // ball update
    b_xPosition += b_xSpeed * b_xDirection;
    b_yPosition += b_ySpeed * b_yDirection;
    
    if (b_xPosition > (ofGetWindowWidth() - b_radius) || b_xPosition < b_radius) {
        b_xDirection *= -1;
    }
    if (b_yPosition > (ofGetWindowHeight() - b_radius) || b_yPosition < b_radius) {
        b_yDirection *= -1;
    }
    
    
    // 공과 하키채가 만나면(=충돌하면) 튕겨야 한다
    checkCollision(m_ImgPositionX1, m_ImgPositionY1);
    checkCollision(m_ImgPositionX2, m_ImgPositionY2);
  
    
}

//--------------------------------------------------------------
void ImageExample::draw(){

// 	myTuio.drawCursors();
// 	myTuio.drawObjects();

    
    m_BgImage.draw(0,0);

    
    // 배경에 선 그리기
    ofSetColor(255,0,0);
    ofLine(WINDOW_WIDTH/2,0,WINDOW_WIDTH/2,WINDOW_HEIGHT); // 중앙선
    ofNoFill();
    ofCircle(WINDOW_WIDTH/2,WINDOW_HEIGHT/2,150); // 중앙 원 선
    ofCircle(0,WINDOW_HEIGHT/2,150); // 왼쪽 원 선
    ofCircle(WINDOW_WIDTH,WINDOW_HEIGHT/2,150); // 오른쪽 원 선
    
    ofSetColor(255,0,0,50);
    ofFill();
    ofCircle(0,WINDOW_HEIGHT/2,150); // 왼쪽 원
    ofCircle(WINDOW_WIDTH,WINDOW_HEIGHT/2,150); // 오른쪽 원
    
    ofSetColor(255,255,255);
    
    ofLine(WINDOW_WIDTH-30.0, 10, WINDOW_WIDTH-30.0, 50);
    
    
    
    // player1
    m_OpenGLRenderer.pushMatrix();
    m_OpenGLRenderer.translate(m_ImgPositionX1, m_ImgPositionY1);
    m_OpenGLRenderer.rotateZ(m_ImgRotateAngle);
    
    m_OpenGLRenderer.draw(m_player1, -m_ImgWidth / 2, -m_ImgHeight / 2, 0.0,
                          m_ImgWidth, m_ImgHeight,
                          0.0, 0.0,
                          m_player1.getWidth(), m_player1.getHeight());
    m_OpenGLRenderer.popMatrix();
    
    
    // player2
    m_OpenGLRenderer.pushMatrix();
    m_OpenGLRenderer.translate(m_ImgPositionX2, m_ImgPositionY2);
    m_OpenGLRenderer.rotateZ(m_ImgRotateAngle);

    m_OpenGLRenderer.draw(m_player2, -m_ImgWidth / 2, -m_ImgHeight / 2, 0.0,
                          m_ImgWidth, m_ImgHeight,
                          0.0, 0.0,
                          m_player2.getWidth(), m_player2.getHeight());
    
    m_OpenGLRenderer.popMatrix();
    
    
    
    //ball draw
    ofFill();
    ofCircle(b_xPosition, b_yPosition, b_radius);
    
    
    // 경고 유무
    if (caution == true){
        m_CautionImage.draw(WINDOW_WIDTH/2-250,WINDOW_HEIGHT/2-250);
    }
    
    
    
    // 공과 골대가 만나면 점수가 증가한다
    if (b_xPosition <= 30.0 && b_yPosition >= WINDOW_HEIGHT/2-150.0 && b_yPosition <= WINDOW_HEIGHT/2+150.0){
        scoreValue2 += 1;
        goalEffect2 = true;
    }
    else if (b_xPosition >= WINDOW_WIDTH-30.0 && b_yPosition >= WINDOW_HEIGHT/2-150.0 && b_yPosition <= WINDOW_HEIGHT/2+150.0){
        scoreValue1 += 1;
        goalEffect1 = true;
    }
    
    // 점수 그리기
    score1 = ofToString(scoreValue1);
    score2 = ofToString(scoreValue2);
    font.drawString(score1,WINDOW_WIDTH/4-50,WINDOW_HEIGHT/2+50); // 왼쪽 구역 중앙
    font.drawString(score2,WINDOW_WIDTH/4*3-50,WINDOW_HEIGHT/2+50); // 오른쪽 구역 중앙
    
    
    // 득점 이펙트 (거창한 효과는 못했지만....)
    if (goalEffect2 == true){
        font.drawString("Goal!!!", WINDOW_WIDTH/4*3-200, WINDOW_HEIGHT/2-100);
        excellent.play();
        goalEffect2 = false;
    }
    if (goalEffect1 == true){
        font.drawString("Goal!!!", WINDOW_WIDTH/4-200, WINDOW_HEIGHT/2-100);
        excellent.play();
        goalEffect1 = false;
    }
}

//--------------------------------------------------------------
void ImageExample::keyPressed(int key){

}

//--------------------------------------------------------------
void ImageExample::keyReleased(int key){

}

//--------------------------------------------------------------
void ImageExample::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ImageExample::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ImageExample::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ImageExample::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ImageExample::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ImageExample::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ImageExample::dragEvent(ofDragInfo dragInfo){ 
	
}

void ImageExample::touchDown( ofTouchEventArgs & touch )
{
}

void ImageExample::touchMoved( ofTouchEventArgs & touch )
{
}

void ImageExample::touchUp( ofTouchEventArgs & touch )
{

}

void ImageExample::checkCollision(float m_ImgPositionX, float m_ImgPositionY) {
    
    float distance = ofDist(m_ImgPositionX,m_ImgPositionY,b_xPosition,b_yPosition);
    float minDistance = m_ImgWidth/2 + b_radius/2;
    
    float dx = m_ImgPositionX - b_xPosition;
    float dy = m_ImgPositionY - b_yPosition;
    
    if (distance < minDistance) { // 충돌
        float angle = atan2(dy,dx);
        float targetX = b_xPosition + cos(angle) * minDistance;
        float targetY = b_yPosition + sin(angle) * minDistance;
        float ax = (targetX - m_ImgPositionX) * spring;
        float ay = (targetY - m_ImgPositionY) * spring;
        
        b_xDirection -= ax;
        b_yDirection -= ay;
        
        pop.play();
        
    }
   
}
