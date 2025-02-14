@echo off

windres -i  resource.rc -o resource.o

g++ -o "math_tools" main.cpp resource.o -mwindows -lgdi32 -finput-charset=UTF-8 -Wall -static -static-libgcc -static-libstdc++ -lstdc++ -lgcc -lwinpthread -ffunction-sections -fdata-sections -Wl,--gc-sections

