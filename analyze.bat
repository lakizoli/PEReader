del /Q "d:\work\analyze_testhw\*.*"
del /Q "d:\work\analyze_c1_64bit\*.*"
del /Q "d:\work\analyze_c1xx_64bit\*.*"
del /Q "d:\work\analyze_c2_64bit\*.*"
del /Q "d:\work\analyze_c1_32bit\*.*"
del /Q "d:\work\analyze_c1xx_32bit\*.*"
del /Q "d:\work\analyze_c2_32bit\*.*"
del /Q "d:\work\analyze_ucrtbase\*.*"

x64\Release\runflat analyze "c:\work\git_repos\PEReader\x64\Release\testhw.exe.flat" "d:\work\analyze_testhw"
x64\Release\runflat analyze "c:\work\git_repos\PEReader\x64\Release\c1_64bit.dll.flat" "d:\work\analyze_c1_64bit"
x64\Release\runflat analyze "c:\work\git_repos\PEReader\x64\Release\c1xx_64bit.dll.flat" "d:\work\analyze_c1xx_64bit"
x64\Release\runflat analyze "c:\work\git_repos\PEReader\x64\Release\c2_64bit.dll.flat" "d:\work\analyze_c2_64bit"
x64\Release\runflat analyze "c:\work\git_repos\PEReader\x64\Release\c1_32bit.dll.flat" "d:\work\analyze_c1_32bit"
x64\Release\runflat analyze "c:\work\git_repos\PEReader\x64\Release\c1xx_32bit.dll.flat" "d:\work\analyze_c1xx_32bit"
x64\Release\runflat analyze "c:\work\git_repos\PEReader\x64\Release\c2_32bit.dll.flat" "d:\work\analyze_c2_32bit"
x64\Release\runflat analyze "c:\work\git_repos\PEReader\x64\Release\ucrtbase.dll.flat" "d:\work\analyze_ucrtbase"
