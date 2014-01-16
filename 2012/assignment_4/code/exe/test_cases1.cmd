@echo off
if not exist out1 mkdir out1
del out1\*.png


..\bin\four_x64_Release -input scenes1/scene1_01.txt -size 200 200 -output out1/output1_01.png -depth 9 10 out1/depth1_01.png -normals out1/normal1_01.png
..\bin\four_x64_Release -input scenes1/scene1_02.txt -size 200 200 -output out1/output1_02.png -depth 8 12 out1/depth1_02.png
..\bin\four_x64_Release -input scenes1/scene1_03.txt -size 200 200 -output out1/output1_03.png -depth 8 12 out1/depth1_03.png
..\bin\four_x64_Release -input scenes1/scene1_04.txt -size 200 200 -output out1/output1_04.png -depth 12 17 out1/depth1_04.png
..\bin\four_x64_Release -input scenes1/scene1_05.txt -size 200 200 -output out1/output1_05.png -depth 14.5 19.5 out1/depth1_05.png
..\bin\four_x64_Release -input scenes1/scene1_06.txt -size 200 200 -output out1/output1_06.png -depth 3 7 out1/depth1_06.png
..\bin\four_x64_Release -input scenes1/scene1_07.txt -size 200 200 -output out1/output1_07.png -depth -2 2 out1/depth1_07.png
..\bin\four_x64_Release -input scenes1/scene2_01_diffuse.txt -size 200 200 -output out1/output2_01.png
..\bin\four_x64_Release -input scenes1/scene2_02_ambient.txt -size 200 200 -output out1/output2_02.png
..\bin\four_x64_Release -input scenes1/scene2_03_colored_lights.txt -size 200 200 -output out1/output2_03.png -normals out1/normals2_03.png
..\bin\four_x64_Release -input scenes1/scene2_04_perspective.txt -size 200 200 -output out1/output2_04.png -normals out1/normals2_04.png
..\bin\four_x64_Release -input scenes1/scene2_06_plane.txt -size 200 200 -output out1/output2_06.png -depth 8 20 out1/depth2_06.png -normals out1/normals2_06.png
..\bin\four_x64_Release -input scenes1/scene2_08_cube.txt -size 200 200 -output out1/output2_08.png
..\bin\four_x64_Release -input scenes1/scene2_09_bunny_200.txt -size 200 200 -output out1/output2_09.png
..\bin\four_x64_Release -input scenes1/scene2_10_bunny_1k.txt -size 200 200 -output out1/output2_10.png
REM requires transform
rem ..\bin\four_x64_Release -input scenes1/scene2_11_squashed_sphere.txt -size 200 200 -output out1/output2_11.png -normals out1/normals2_11.png
rem ..\bin\four_x64_Release -input scenes1/scene2_12_rotated_sphere.txt -size 200 200 -output out1/output2_12.png -normals out1/normals2_12.png
rem ..\bin\four_x64_Release -input scenes1/scene2_13_rotated_squashed_sphere.txt -size 200 200 -output out1/output2_13.png -normals out1/normals2_13.png
rem ..\bin\four_x64_Release -input scenes1/scene2_14_axes_cube.txt -size 200 200 -output out1/output2_14.png
rem ..\bin\four_x64_Release -input scenes1/scene2_15_crazy_transforms.txt -size 200 200 -output out1/output2_15.png
rem ..\bin\four_x64_Release -input scenes1/scene2_16_t_scale.txt -size 200 200 -output out1/output2_16.png -depth 2 7 out1/depth2_16.png
