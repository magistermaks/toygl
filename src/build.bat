
mkdir build
g++ -O0 -g examples/%1.cpp common/wxgl.c -I. -lopengl32 -lgdi32 -o build/%1.exe && ./build/%1.exe

