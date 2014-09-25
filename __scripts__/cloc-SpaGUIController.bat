REM Run cloc tool http://cloc.sourceforge.net/ to count the lines of code

..\__tools__\cloc-1.62.exe ..\SpaGUIController\ --by-file-by-lang --exclude-ext=pro,ts,xml --report-file=..\__reports__\cloc-SpaGUIController.txt
..\__tools__\cloc-1.62.exe ..\SpaGUIController\ --by-file-by-lang --exclude-lang="TypeScript","C","C++","C/C++ Header","IDL" -exclude-ext=xml --force-lang=xml,ui --report-file=..\__reports__\cloc-SpaGUIController-ui.txt
..\__tools__\cloc-1.62.exe ..\SpaGUIController\ --by-file-by-lang --exclude-lang="TypeScript","C","C++","C/C++ Header","IDL" -exclude-ext=xml --force-lang=xml,ts --report-file=..\__reports__\cloc-SpaGUIController-ts.txt

REM Extensions
REM IDL        - .idl, .pro
REM TypeScript - .ts








