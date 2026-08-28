@REM set "EXTERNAL_INCLUDE=include/"
@REM set "LIB_PATH=lib/"

g++ -I"../include/" -L"../lib/" ^
../LevelEditor/*.cpp ^
../LevelEditor/Map/*.cpp ^
../include/ImGUI/*.cpp ^
../include/rlImGUI/*.cpp ^
-lraylib ^
-lcomdlg32 ^
-lwinmm ^
-lgdi32 ^
-lopengl32 ^
-luser32 ^
-lkernel32 ^
-lLevelEditor ^
-mwindows ^
-o ../Build/LevelEditor.exe

echo All Done