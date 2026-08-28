@REM set "EXTERNAL_INCLUDE=include/"
@REM set "LIB_PATH=lib/"

@REM g++ -c -DBUILD_DLL -I"../include/" ../LevelEditor/dllstuff/Map.cpp -o ../LevelEditor/dllstuff/Map.o 
g++ -c -DBUILD_DLL -I"../include/" ../LevelEditor/dllstuff/defaultmap.cpp -o ../LevelEditor/dllstuff/defaultmap.o 
g++ -c -DBUILD_DLL -I"../include/" ../LevelEditor/dllstuff/something.cpp -o ../LevelEditor/dllstuff/something.o 

g++ -shared -o ../Build/LevelEditor.dll ../LevelEditor/dllstuff/*.o -L"../lib/" -lraylib -lcomdlg32 -lwinmm -lgdi32 -lopengl32 -luser32 -lkernel32 -Wl,--out-implib,../lib/libLevelEditor.a

echo All Done