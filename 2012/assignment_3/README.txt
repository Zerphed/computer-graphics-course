Name: Joonas Nissinen
Student id: ******
Assignment: Assignment 3


COLLABORATION:
--------------

I had some collaboration with Paula Jukarainen. Some collaboration was also done on IRC. The discussion was strictly on an abstract level, about how some of the things could be implemented smartly, and no code was shared.


REFERENCES:
--------------

Lecture slides
Computer Graphics with OpenGL, Fourth Edition, By: Hearn, Baker and Carithers
Advanced Engineering Mathematics, Ninth Edition, By: Erwin Kreyzig

http://en.wikipedia.org/wiki/Runge_Kutta
http://www.cplusplus.com/reference/std/random/normal_distribution/


KNOWN PROBLEMS:
---------------

There is the fact that the code can be a bit slow at times, but I tried to optimize it without jeopardizing the readability of the code.

Another small issue is that I wasn't able to make the GUI button for the wind emulation recognize when the emulation is on (button pressed) and when its not (button not pressed). Nonetheless pressing the button or 'W' triggers the effect.

Lastly there are some unsigned vs. signed comparions that the compiler gives warnings for.


EXTRA CREDIT:
---------------

I implemented the Runge-Kutta (RK4) Integrator. I can hardly think of it as extra credit as it was almost a necessity to be able to debug the cloth. I also implemented the (very simple) wind emulation using C++ random library and its normal distributions.


COMMENTS:
---------------

I used about 10-11 hours for this exercise, mainly due to some funny problems with the cloth.

As a side note I think you should fix the inheritance hierarchy where the cloth system inherits the pendulum system. When inheriting the pendulum system the cloth system initializes all the pendulum system's variables redundantly. I suggest you make an abstract class in between the two called AbstractPendulumSystem or something. 

The instructions concerning the drawing of faces could also be improved and the redundant comments about vecv and vecn be removed from the clothSystem.h.

Furthermore I think some of the public values in the cloth system class could be private and changed them to such accordingly.

All in all, I liked this exercise also and most of it was well tought out and implemented.

P.S. I think the left and right were reversed in moving the pendulum system and reversed them in my own solution.

