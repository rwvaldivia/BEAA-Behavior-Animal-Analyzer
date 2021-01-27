CLS
@ECHO OFF
SET _JAVA_DIR_SRC_TO_GENERATE_H_=C:\Java\Projetos_Mestrado\ImageProc\src\
SET _C_PROJECT_DLL_="C:\Users\lucianav\Documents\Visual Studio 2010\Projects\ImageProcDLL\ImageProcDLL"
SET _C_FILE_GENERATED_=*.h
SET _CLASS_FILE_GENERATE_H_=br.com.protowork.engine.ImageProcessContainer
c:
cd \
cd %_JAVA_DIR_SRC_TO_GENERATE_H_%
ECHO GERANDO ARQUIVOS H

javah %_CLASS_FILE_GENERATE_H_%
copy %_C_FILE_GENERATED_% %_C_PROJECT_DLL_%
  
pause