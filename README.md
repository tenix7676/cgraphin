# cgraphin
The program is a multi-threaded graphin calculator.

To set the function you need to edit code though.
line 147 function `double f(double x)` will be graphed


The graph shows that the crazy Ramanujan's formula
$$
R_n(x)=\cfrac{1}{x+\cfrac{1}{x+\cfrac{2}{x+\cfrac{3}{x+...}}}}
$$
approaches $\sqrt{\pi/2}$ at x=0 if you take the geometric mean of 
its odd and even expansions - which is sort of just the square root of the Wallis product.

Even expansion means it ends in an even number:
$
R_4=\cfrac{1}{x+\cfrac{1}{x+\cfrac{2}{x+\cfrac{3}{x+4}}}}
$
$
f(x)=\frac{1}{x}
$

The function being plotted is $f(x)=\sqrt{R_{n} R_{n+1}}$
where n = 100.

Of course I was inspired by Mathologer's ["Ramanujan's easiest hard infinity monster (Mathologer Masterclass)"](https://www.youtube.com/watch?v=6iTdNmDHfV0)

# compiling
If you have Visual Studio installed you should have a shortcut called "x64 Native Tools Command Prompt for VS 2022" on your pc. Open that shortcut and navigate to the project's directory.
Run `cl.exe /I./ cgraphin.c /link /defaultlib:sdl3 /subsystem:console && cgraphin.exe`.
