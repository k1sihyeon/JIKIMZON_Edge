# [JIKIMZON](https://github.com/VEDA-tmpa)_Edge


## 개발 환경
- Raspberry Pi 5
- Raspberry Pi Camera Rev 1.3


## 튜토리얼
### ① 라이브러리 설치
``` bash
sudo apt-get update
sudo apt-get install -y \
    libopencv-dev \
    libavcodec-dev \
    libavformat-dev \
    libavutil-dev \
    libswscale-dev \
    libssl-dev \
    libcrypto++-dev
```


### ② 빌드
``` bash
git clone https://github.com/VEDA-tmpa/JIKIMZON_Edge.git
cd JIKIMZON_Edge
./build.sh
```


### ③ 실행
``` bash
./edge.out <frame port num> <json port num>
```
or
``` bash
export LD_PRELOAD=/usr/lib/aarch64-linux-gnu/libcamera/v4l2-compat.so
./edge.out <frame port num> <json port num>
```
or
``` bash
LD_PRELOAD=/usr/lib/aarch64-linux-gnu/libcamera/v4l2-compat.so ./edge.out <frame port num> <json port num>
```


## 스택
- Language   
  - C++

- Libraries   
  - opencv (4.6.0)
  - openssl (3.0.15)
  - ffmpeg (5.1.6)
  - nlohmann_json (3.11.3)
   
- Model
  - [yolov5n-garbage](https://huggingface.co/keremberke/yolov5n-garbage) (converted into onnx format)


## 기능
- 쓰레딩을 통한 프레임 전처리
- H.264 인코딩
- 쓰레기 객체 탐지
- 데이터 직렬화
- TLS 송신   


## 프로젝트 구조
```
.
├── Makefile
├── README.md
├── build.sh
├── certs
│   ├── server.cert
│   └── server.key
├── inc
│   ├── ISerializeInterface.hpp
│   ├── body.hpp
│   ├── captureHandler.hpp
│   ├── cipherHandler.hpp
│   ├── encodeHandler.hpp
│   ├── frame.hpp
│   ├── header.hpp
│   ├── objectHandler.hpp
│   ├── preprocessHandler.hpp
│   ├── tcpHandler.hpp
│   ├── tlsHandler.hpp
│   └── utils.hpp
├── lib
│   └── libcapture
├── res
│   ├── yolov5m-garbage.onnx
│   ├── yolov5n-garbage.onnx
│   └── yolov5s-garbage.onnx
├── src
│   ├── body.cpp
│   ├── captureHandler.cpp
│   ├── cipherHandler.cpp
│   ├── encodeHandler.cpp
│   ├── frame.cpp
│   ├── header.cpp
│   ├── keyfile.bin
│   ├── keyfile2.bin
│   ├── main.cpp
│   ├── objectHandler.cpp
│   ├── preprocessHandler.cpp
│   ├── tcpHandler.cpp
│   ├── tlsHandler.cpp
│   └── utils.cpp
├── test
│   └── tutorial
│       ├── opencv
│       │   ├── lenna.jpg
│       │   ├── main.cpp
│       │   ├── main.out
│       │   ├── mandrill.png
│       │   ├── mandrill2.jpg
│       │   ├── output.h264
│       │   ├── preprocessHandler.cpp
│       │   ├── preprocessHandler.hpp
│       │   └── temp.jpg
│       ├── openssl
│       │   ├── Makefile
│       │   ├── cipherHandler.cpp
│       │   ├── cipherHandler.hpp
│       │   ├── keyfile.bin
│       │   ├── main.cpp
│       │   └── openssl
│       ├── serialize
│       │   └── main.cpp
│       └── tlsTest
│           ├── client
│           │   ├── Makefile
│           │   ├── client.cpp
│           │   ├── tcp.cpp
│           │   ├── tcp.hpp
│           │   ├── tls.cpp
│           │   └── tls.hpp
│           └── server
│               ├── server.cpp
│               ├── tcp.cpp
│               ├── tcp.hpp
│               ├── tls.cpp
│               └── tls.hpp
└── video
    └── original.h264

17 directories, 62 files
```

