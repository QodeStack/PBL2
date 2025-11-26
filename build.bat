@echo off
g++ -std=c++17 main.cpp models/*.cpp controllers/*.cpp views/*.cpp storage/*.cpp -o main
pause
