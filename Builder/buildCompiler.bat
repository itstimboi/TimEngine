@REM set "EXTERNAL_INCLUDE=include/"
@REM set "LIB_PATH=lib/"

"../../../msys64/ucrt64/bin/g++.exe" -I"../include/" -L"../lib/" ^
build.cpp ^
../include/ImGUI/*.cpp ^
../include/rlImGUI/*.cpp ^
-lraylib ^
-lwinmm ^
-lgdi32 ^
-lopengl32 ^
-luser32 ^
-lkernel32 ^
-mwindows ^
-o build.exe

echo All Done