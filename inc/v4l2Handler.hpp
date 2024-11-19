#ifndef JIKIMZON_V4L2HANDLER_H
#define JIKIMZON_V4L2HANDLER_H

#include <iostream>

class V4L2Handler {
public:
    V4L2Handler();
    ~V4L2Handler();

    void StartCapture();

    static V4L2Handler* GetInstance()
    {
        if (instance == NULL)
        {
            instance = new V4L2Handler();
        }
        return instance;
    }

private:
    static V4L2Handler* instance;

};

#endif