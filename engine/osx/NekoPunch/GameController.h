//
//  GameController.h
//  NyanTest4
//
//  Created by Cartwright Samuel on 9/26/12.
//  Copyright (c) 2012 Cartwright Samuel. All rights reserved.
//

#ifndef __NyanTest4__GameController__
#define __NyanTest4__GameController__

#include "glUtil.h"


namespace OSXInputDeviceID {
	enum {
		DEVICE,
		TOTAL,
	};
}

namespace OSXInputDeviceSensorID {
	enum {
		KEYBOARD,
		POINTER,
		MOUSE_LEFT,
		MOUSE_MIDDLE,
		MOUSE_RIGHT,
		TOTAL,
	};
}

class GameController
{
public:
    GameController(unsigned int screenWidth, unsigned int screenHeight);
    ~GameController();
    
    void initialise();
    void destroy();
    
    void update(float dt);
    void draw();
    
    inline bool isInitialized() {return m_bIsInitialized;}
    
    void keyDown(int key);
    void keyUp(int key);
    void mouseDown(int button, int x, int y );
    void mouseUp(int button, int x, int y );
    void mouseDrag(int x, int y);
    void mouseMove(int x, int y);
    void scrollWheel(float wheelDelta);


private:
    void setupGL();
    void setupFramebuffer();
    void setupLighting();
    void setupGeometry();
    void setupWorld();
    void setupPhysics();
    
private:
    bool m_bIsInitialized;
    unsigned int m_screenWidth;        // screen bounds
    unsigned int m_screenHeight;
    
//        unsigned int m_program;
//        
//        mat4 m_ProjectionMatrix;
//        mat4 m_ViewMatrix;
//        mat4 m_ModelMatrix;
//        mat4 m_CameraMatrix;
//        
//        GLuint m_framebuffer;
//        GLuint m_renderbuffer;
//        GLuint m_depthRenderbuffer;
    
};

#endif /* defined(__NyanTest4__GameController__) */
