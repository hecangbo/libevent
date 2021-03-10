@echo "开始编译libevent"
set VS = "d:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsamd64_x86.bat"
set OUT =E:\Work\kdzb\gitLibevent\libevent\doc\out\vs2017_32\libevent 
call %VS%
cd libevent-master
#清理
nmake /f Makefile.nmake clean
nmake /f Makefile.nmake E:\Work\kdzb\gitLibevent\libevent\doc\out\vs2017_32\openssl

md %OUT%\lib
md %OUT%\bin
md %OUT%\include

copy /Y *.lib %OUT%\lib
xcopy /S/Y include %OUT%\include\
xcopy /S/Y WIN32-Code\nmake\include %OUT%\include\
copy /Y *.dll %OUT%\bin
copy /Y *.exe %OUT%\bin
cd ..
@echo "结束libevent"