@echo off
set DestPath=%~dp0\build\bin

:: choice
@choice /C:rd /M "choose release or debug configuration?"
if errorlevel 2 goto Debug
if errorlevel 1 goto Release
goto end


:Release
XCopy  /s /y /q /h %QTDIR%\bin\Qt5Core.dll                     %DestPath%\
XCopy  /s /y /q /h %QTDIR%\bin\Qt5Gui.dll                      %DestPath%\
XCopy  /s /y /q /h %QTDIR%\bin\Qt5Widgets.dll                  %DestPath%\ 
XCopy  /s /y /q /h %QTDIR%\plugins\platforms\qwindows.dll  %DestPath%\plugins\platforms\
goto end

:Debug
XCopy  /s /y /q /h %QTDIR%\bin\Qt5Cored.dll                     %DestPath%\
XCopy  /s /y /q /h %QTDIR%\bin\Qt5Guid.dll                      %DestPath%\
XCopy  /s /y /q /h %QTDIR%\bin\Qt5Widgetsd.dll                  %DestPath%\ 
XCopy  /s /y /q /h %QTDIR%\plugins\platforms\qwindowsd.dll  %DestPath%\plugins\platforms\
goto end

:end
pause