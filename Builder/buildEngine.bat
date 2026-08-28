@REM set "EXTERNAL_INCLUDE=include/"
@REM set "LIB_PATH=lib/"

g++ -I"../include/" -I"../Engine/" -L".//lib/" ^
../glad.c ^
../Engine/*.cpp ^
../Engine/Camera/*.cpp ^
../Engine/Rendering/*.cpp ^
../Engine/Textures/*.cpp ^
../Engine/Light/*.cpp ^
../Engine/Mesh/*.cpp ^
../Engine/Map/*.cpp ^
../Engine/Sprites/*.cpp ^
../Engine/Audio/*.cpp ^
../include/ImGUI/*.cpp ^
-lglfw3 -lgdi32 -lopengl32 -lopenal -luser32 -lkernel32 ^
-o ../Build/Engine.exe

echo All Done

@REM Engine/Audio/stb_vorbis.c ^