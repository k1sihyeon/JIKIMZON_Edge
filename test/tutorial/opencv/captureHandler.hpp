#ifndef JIKIMZON_CAPTUREHANDLER_TEST_H
#define JIKIMZON_CAPTUREHANDLER_TEST_H

#include <iostream>

namespace size
{
    const int WIDTH = 1280;
    const int HEIGHT = 720;
}

class CaptureHandler
{
public:
    CaptureHandler(const CaptureHandler&) = delete;             // 복사 생성자 삭제
    CaptureHandler& operator=(const CaptureHandler&) = delete;  // 복사연산자 삭제

    static CaptureHandler& GetInstance();  // 인스턴스 반환 static 함수
    void StartCapture();                   // 카메라로 부터 캡쳐를 시작하는 함수

private:
    CaptureHandler() = default;  // default 생성자, 소멸자를 private으로 선언하여 외부에서의 생성자/소멸자 호출 X
    ~CaptureHandler() = default;
    static CaptureHandler* sInstance;
};

#endif