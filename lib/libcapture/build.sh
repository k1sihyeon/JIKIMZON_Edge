g++ -c libcapture.cpp -o libcapture.o `pkg-config --cflags opencv4`
g++ -shared -o libcapture.so libcapture.o `pkg-config --libs opencv4`
gcc -o main main.c -L. -lcapture