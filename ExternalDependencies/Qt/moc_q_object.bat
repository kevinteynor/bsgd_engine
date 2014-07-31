@ECHO OFF

set proDir=%1
set qtMocLoc=%2
set proDir=%proDir:"=%
set qtMocLoc=%qtMocLoc:"=%

FOR %%d in ("%proDir%*.h") DO (
	FIND /C "Q_OBJECT" "%%d"
	IF NOT ERRORLEVEL 1 CALL "%qtMocLoc%" "%%d" > "%proDir%%%~nd_moc.cpp"
)

EXIT 0