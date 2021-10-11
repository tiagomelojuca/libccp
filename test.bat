@echo off
call install
.\build\test\test_minneg.exe --gtest_color=yes
.\build\test\test_punc.exe   --gtest_color=yes
