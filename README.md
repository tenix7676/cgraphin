# cgraphin
The program is a multi-threaded graphin calculator.

To set the function you need to edit code though.

# compiling
If you have Visual Studio installed you should have a shortcut called "x64 Native Tools Command Prompt for VS 2022" on your pc. Open that shortcut and navigate to the project's directory.
Run `cl.exe /I./ cgraphin.c /link /defaultlib:sdl3 /subsystem:console && cgraphin.exe`.
