CLS
@ECHO OFF
SET _DLL_FILE_="C:\Users\lucianav\Documents\Visual Studio 2010\Projects\ImageProcDLL\x64\Debug\ImageProcDLL.dll"
SET _DEST_DLL_FILE_="C:\ImageProc\dll"

SET _JAR_FILE_="C:\Java\Projetos_Mestrado\ImageProc\dist\ImageProc.jar"
SET _DEST_JAR_FILE_="C:\ImageProc\app"

c:
cd \

ECHO Copiando arquivos

copy %_DLL_FILE_% %_DEST_DLL_FILE_%
copy  %_JAR_FILE_% %_DEST_JAR_FILE_%
copy "C:\Users\lucianav\Documents\Visual Studio 2010\ImageProc.bat" "c:\ImageProc"
  
pause