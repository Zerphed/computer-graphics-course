Name: Joonas Nissinen
Student id: ******
Assignment: Assignment 2


COLLABORATION:
--------------

I had some collaboration with Paula Jukarainen, but the collaboration was strictly on an abstract level. However some coding happened in the same room so some similarities might have risen.


REFERENCES:
--------------

Lecture slides ;)
Computer Graphics with OpenGL, Fourth Edition, By: Hearn, Baker and Carithers


KNOWN PROBLEMS:
---------------

There are no problems in the code as far as I know. 

..Ok there is ONE small issue. The results of the print screens started looking weird after I had implemented the coloring for the model :|. Also the implementation of my color to joint mapping allows same colors for joints in models with very many joints (>30 joints).


EXTRA CREDIT:
---------------

I implemented the coloring to the vertices according to weighted average of the joint weights affecting that vertex. The colors of the joints are also visible in the skeleton mode. I had to sort of play the color of the bones by ear since I couldn't find the original one.


ARTIFACTS:
---------------

The artifact that I created can be found in the root of the .zip archive named as artifact.png. Hopefully that counts as an interesting pose :p.


COMMENTS:
---------------

I used about 8-9 hours for this exercise. I think the assignment and the given code was very clearly structured, apart from a couple of errors in the comments.

I think the bindWorldToJointTransform and currentJointToWorldTransform could have been explained a little bit better, it took some time to figure out their role and which is which. Another problem that I had was with the order of transformations in the drawSkeletonRecurse function. I think the exercise description was somewhat misleading about the correct applying order of the transformations.

All in all I liked the exercise, and I feel that this was a wee bit easier than the last one.
