SET JAVA_HOME=C:\ImageProc\jre\bin
SET IMAGE_PROC_APP_DIR=C:\ImageProc\app
SET IMAGE_PROC_DEPENDENT_DLL_DIR=C:\ImageProc\dll
SET PATH=%PATH%;%IMAGE_PROC_DEPENDENT_DLL_DIR%;%IMAGE_PROC_DEPENDENT_DLL_DIR%\opencv248\x64\vc10\bin\
CLS
%JAVA_HOME%\java.exe -Djava.library.path=%IMAGE_PROC_DEPENDENT_DLL_DIR%\opencv248\x64\vc10\bin\ -jar %IMAGE_PROC_APP_DIR%\ImageProc.jar
