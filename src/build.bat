
mkdir build

set file=%1
g++ -O0 -g examples/%file%.cpp common/wxgl.c -I. -lopengl32 -lgdi32 -o build/%file%.exe && build\%file%.exe

