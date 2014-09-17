REM Run cloc tool http://cloc.sourceforge.net/ to count the lines of code

..\__tools__\cloc-1.62.exe ..\GUIController\ --by-file-by-lang --exclude-ext=pro,ts,xml --report-file=..\__reports__\cloc-GUIController.txt
..\__tools__\cloc-1.62.exe ..\GUIController\ --by-file-by-lang --exclude-lang="TypeScript","C","C++","C/C++ Header","IDL" -exclude-ext=xml --force-lang=xml,ui --report-file=..\__reports__\cloc-GUIController-ui.txt
..\__tools__\cloc-1.62.exe ..\GUIController\ --by-file-by-lang --exclude-lang="TypeScript","C","C++","C/C++ Header","IDL" -exclude-ext=xml --force-lang=xml,ts --report-file=..\__reports__\cloc-GUIController-ts.txt

REM Extensions
REM IDL        - .idl, .pro
REM TypeScript - .ts








