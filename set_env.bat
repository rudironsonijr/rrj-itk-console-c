set "p7mgrendene_dir=%cd%"
echo "p7mgrendene_dir: %p7mgrendene_dir%"

if not defined TC_ROOT set TC_ROOT=C:\SPLM\TC12
if exist "%TC_ROOT%\install\tem_init.bat" call "%TC_ROOT%\install\tem_init.bat"

REM if not defined HTTP_SERVER_PATH set HTTP_SERVER_PATH=~your_httpserver_dir~
if not defined IIOP_SERVER_PATH set IIOP_SERVER_PATH=C:\SPLM\TC12\iiopservers
if not defined JAVA_HOME set JAVA_HOME=C:\Progra~1\Zulu\zulu-8
if not defined JRE_HOME set JRE_HOME=%TC_JRE_HOME%
if not defined JDK_HOME set JDK_HOME=C:\Progra~1\Zulu\zulu-8
if not defined FMS_HOME set FMS_HOME=C:\SPLM\TC12\tccs


CALL %TC_DATA%\tc_profilevars.bat
CALL "C:\APPS\Microsoft\VisualStudio\BuildTools\2017\VC\Auxiliary\Build\vcvars64.bat"
