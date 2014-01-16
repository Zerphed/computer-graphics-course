Name: Joonas Nissinen
Student ID: ******
Assignment: Assignment 4 - Ray Tracer
Date: 9.12.2012

COLLABORATION:
--------------

Some collaboration was done with Paula Jukarainen, mainly helping on my part. I also gave advice to several people in the IRC, and asked a couple of questions in Piazza.


REFERENCES:
--------------

There are far too many references to list but I'll try to paste some of the ones I used:

http://www.cosy.sbg.ac.at/~held/teaching/graphik/projekte_2006/3d_textures_slides.pdf
http://www.cs.washington.edu/education/courses/cse457/07sp/lectures/triangle_intersection.pdf
http://devmag.org.za/2009/04/25/perlin-noise/
http://www.scratchapixel.com/lessons/3d-basic-lessons/lesson-7-intersecting-simple-shapes/ray-box-intersection/

To add to those there were multiple wikipedia articles about intersections and refraction.

One book I used was Computer Graphics with OpenGL by Hearn and Baker.


KNOWN PROBLEMS:
----------------

There aren't any problems with the code that I am aware of. At least for the minimum requirements.

For the extra credit however, I think there might be some problems with my Fresnel coefficients (stuff gets darker when using them) and I didn't manage to finish my implementation for Perlin noise.


EXTRA CREDIT:
----------------

I implemented the following extra credit at least to some extent in my assignment:

Transformations: I implemented transformations and they seem to work fine for all the scenes I am aware of.

Box: I implemented the Box primitive, including the parser implementation and intersect. You can view one scene making use of the box primitive by using the scene box_ambient.txt found under /bin/extras/. There is also a preview of the scene included.

Refraction: I implemented refraction to the best of my ability in the code. This took quite a lot of work to get right (sadly). I also accounted for the TIR.

Fresnel: I implemented the Fresnel coefficients for refraction and reflection, however I am unsure whether I got them right since they seem to make the scenes darker. You can enable Fresnel in scenes with refraction and reflection by using the "-fresnel" option. The coefficients are computed using the Schlick's approximation.

As to how to fix the problem I think there is something I am missing here since both of the coefficients are < 1.0 the scene is bound to be less lighted than before because initially both of the values were summed together with coefficients 1.0. The problem could be probably solved by somehow normalizing the total amount of light, but I couldn't get my head around how to do it, not because of lack of trying though. All in all this might be physically closer to truth so this might not even require fixing in the first place.

Other: I also implemented the parsers for Marble and Noise and began writing the PerlinNoise class, sadly time ran out and I was unable to finish the class. Thus I got little benefit out of it, I'll improve it in the future tough!


COMMENTS:
--------------

Best regards. I think next year you should try to streamline the code a bit and try to document the variables better. Sometimes it took a lot of time to understand the structure.