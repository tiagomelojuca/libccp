@echo off
del .\build\test\test_minneg.exe
del .\build\test\test_punc.exe
call install
.\build\test\test_minneg.exe --gtest_color=yes
.\build\test\test_punc.exe   --gtest_color=yes
