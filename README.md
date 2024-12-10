# JIKIMZON_Edge

## Run
### Install Libraries
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

### Build
``` bash
git clone https://github.com/VEDA-tmpa/JIKIMZON_Edge.git
cd JIKIMZON_Edge
./build.sh
```

### Run
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
