Name: Joonas Nissinen
Student id: ******
Assignment: Assignment 0

README
------

Used environment: Windows 7 Professional x64 with SP1
Used IDE........: Microsoft Visual Studio 2010 Professional with SP1
Graphics card...: Nvidia Geforce G210M
Driver version..: 186.88


Collaboration:
--------------

I had some discussion about the exercise and its contents with Paula Jukarainen. However, since no code was shown by either one, I'd imagine that there aren't any similarities.


References:
---------------

Since the exercise was mostly concerned with basic stuff I didn't need too many references. However there were some websites and books I used:

Books:

Computer Graphics with OpenGL (4th Edition), Hearn, Baker, Carithers - ISBN-10: 0132484579

Websites:

http://www.opengl.org/sdk/docs/
http://www.lighthouse3d.com/opengl/displaylists/
http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Arcball


Problems with the code:
------------------------

There shouldn't be any major problems that I am aware of. One "bug" is that when you zoom to the other side of the model (i.e. move the camera to the other side), the camera in a way rotates 180 degrees in order to look at the same point as from the other side. 

If one sees this as a problem it could be fixed by creating a bounding sphere for the model and restricting the zoom from going within the model. Another way would be to simply check if the camera passed to the other side of the model and fix where the camera looks manually. However, as I didn't see this as a major bug and decided not to mess with it.

Other than that, the rotation seemed to be a bit laggy every now and then, but it might be due to my own machine, because I did implement timers to keep the rotation constant.

I didn't have time to finish the object/camera rotation with mouse, but I did create the means to get the arcball vector, in order to get the axis of rotation. The rotateModel already gives sensible results for the rotation axis and angle if you enable the debug prints (however due to buffering other experiences might suffer, such as the rotation of the model using the R key).


Extra credit:
--------------

I did all the extra credit labelled as easy and some features from the medium part:

1. Pressing r rotates the model. I tried to keep the rotation constant using the timer class as suggested.

2. I implemented display lists to achieve higher performance rendering. This didn't require much, mainly a handle for the display list id, stored in the App class, and createDL function that I created. There were also some minor changes due to this e.g. in the renderFrame and destructor functions.

3. Pressing c gives smooth transitions between different colors. This required another time, much like the rotation and some calculation for the delta of the colors. This effected mainly the handleEvent function, and included things like calculating the delta for each color component.

4. I implemented the zoom, which moves the camera from the mouse wheel. The small issue concerning it was described in the problems section. I also started implementing the free object rotation (e.g. getArcballVector), but ran out of time. The closest I got was with the rotateModel, which already gave me sensible results for the rotation axis and the angle.

The extra credit required all in all about 4-5 hours of active work and thinking. The bulk of the time I spent trying to figure out, how the Timer class works. The minimum requirements were implemented in about 1.5 hours.


Comments:
---------------

I had fun doing the exercise, too bad everything piled up on this sunday and I couldn't spend more time working on it. The timer class was a real pain in the ass, but in the end I think I figured it out. I had programmed OpenGL 3.3+, before and learned quite a lot from the fixed pipeline during the exercise.

I feel that the requirements for different parts were clear enough. I am frustrated that I couldn't finish the object rotation though.

P.S. I also fixed the fact that you forgot to close the file stream you opened in the loadMesh function :).