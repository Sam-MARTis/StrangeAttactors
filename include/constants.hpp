#pragma once 

constexpr unsigned int SCREEN_WIDTH = 1000;
constexpr unsigned int SCREEN_HEIGHT = 1000;
constexpr float PI = 3.14159265358979323846f;
constexpr float SCROLL_SENSITIVITY = 0.1f;
constexpr float ROTATION_SENSITIVITY = 0.0015f;

constexpr float MAX_DS = 10.0f;
constexpr float KEYBOARD_MOVE_SPEED = 30.0f;

inline float clamp(float x, float min, float max){
    return x<min?min: x>max?max:x;
}

constexpr unsigned int MAX_PARTICLES = 128*256;
constexpr float DT_FACTOR  = .2f;

constexpr int TRAIL_HISTORY_LENGTH = 2;