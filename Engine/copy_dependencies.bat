@ECHO OFF

set proDir=%1
set extEngineDir=%2
set targetDir=%3
set proDir=%proDir:"=%
set extEngineDir=%extEngineDir:"=%
set targetDir=%targetDir:"=%

FOR %%d in ("%proDir%Debug\*.dll") DO (
	COPY /Y "%%d" "%extEngineDir%bin\"
	COPY /Y "%%d" "%targetDir%"
)

FOR %%d in ("%proDir%*.h") DO (
	COPY /Y "%%d" "%extEngineDir%include\"
)

FOR %%d in ("%proDir%Debug\*.lib") DO (
	COPY /Y "%%d" "%extEngineDir%lib\"
)