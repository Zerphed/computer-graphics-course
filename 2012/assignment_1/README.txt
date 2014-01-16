Name: Joonas Nissinen
Student id: 84362C
Assignment: Assignment 1


COLLABORATION:
--------------

I collaborated with Paula Jukarainen, in the sense that we were coding together at Maarintalo. We also discussed many problems we faced and even solved some of them together. No code was transferred however and both did their own work.

A good example would be when we were figuring how to do the triSweep function. Since neither of us got it right away we collaborated together with paper figuring out the indexing and the kinks.


REFERENCES:
--------------

Computer Graphics with OpenGL, Fourth Edition, By: Hearn, Baker and Carithers
http://en.wikipedia.org/wiki/Cardioid
http://en.wikipedia.org/wiki/Mandelbrot_set


KNOWN PROBLEMS:
---------------

There are no problems with the general requirements that I know of. The mandelbrot artifact that I created, turned out rather unpleasant though. However, I decided to turn it in because I went through some trouble to make it. I could've just drawn some random shape though.


EXTRA CREDIT:
---------------

No extra credit this time folks, I'll try harder next time.


ARTIFACTS:
---------------

The artifacts that I created are in the swp folder in mandelbrot.swp and cardioid.swp. I used python code to generate the point sets. The mandelbrot didn't quite turn out how I hoped, but I hope you can value the effort I put into it.

The screenshots of the artifacts running under my implementation are in the code directory: mandelbrot_screenshot.png, and cardioid_screenshot.png.

cardioid.py:
------------

import math

def main():

    a = 3.0    # Radius
    steps = 49 # Steps

    for i in range(0, steps):
        t = i*((2.0*math.pi)/float(steps));
        x = a*(1.0+2.0*math.cos(t)+math.cos(2.0*t))
        y = a*(2.0*math.sin(t)+math.sin(2.0*t))

        print "[" + str(x) + " " + str(y) + "]"

main()


mandelbrot.py
-------------

import sys

def main():

  height = 25
  width = 75

  # Limit for maximum iterations
  max_iter = 128
  n = 0

  for y in range(0, height):
    for x in range(0, width):

      x0 = (3.5 / width) * x - 2.5
      y0 = (2.0 / height) * y - 1
      xi = 0.0
      yi = 0.0
      iteration = 0

      while (((xi*xi + yi*yi) < 4) and (iteration < max_iter)):

        xtemp = xi*xi - yi*yi + x0
        yi = 2*xi*yi + y0
        xi = xtemp
        iteration = iteration + 1

      if (iteration == max_iter):
        continue
        #sys.stdout.write(' ')

      else:
        case = int((iteration - 1) % 8)
        if case == 4:
          print "[" + str(x/50.0) + " " + str(y/50.0) + "]"
          n += 1

  print n
main()


COMMENTS:
---------------

I think the exercise was generally very well laid out, and the lecture slides helped out a lot. I liked how the assignment went through some of the math involved in making everything happen.

I had problems with figuring out how to do the normals for the curves with the Binit, but solved it eventually. Also remembering the correct transformation of normals in the generalized cylinders took a few minutes.

I feel that I learned a lot from this exercise, but I'm kinda annoyed I didn't get any extra credit done due to hard time at work. It was very rewarding to notice when you had been thinking for hours how to do something, that it turned out to be something so simple and made so much sense.