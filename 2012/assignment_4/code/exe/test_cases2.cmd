@echo off
if not exist out2 mkdir out2
del out2\*.png

..\bin\four_x64_Release -input scenes2\scene3_01_cube_orthographic.txt -size 200 200 -output out2\output3_01s.png 
..\bin\four_x64_Release -input scenes2\scene3_02_cube_perspective.txt -size 200 200 -output out2\output3_02s.png 
..\bin\four_x64_Release -input scenes2\scene3_03_bunny_mesh_200.txt -size 200 200 -output out2\output3_03s.png 
..\bin\four_x64_Release -input scenes2\scene3_03_bunny_mesh_200.txt -size 200 200 -output out2\output3_03super.png -jittered_samples 16
..\bin\four_x64_Release -input scenes2\scene3_04_bunny_mesh_1k.txt -size 200 200 -output out2\output3_04s.png 
REM transform
REM ..\bin\four_x64_Release -input scenes2\scene3_05_axes_cube.txt -size 200 200 -output out2\output3_05s.png 
REM ..\bin\four_x64_Release -input scenes2\scene3_06_crazy_transforms.txt -size 200 200 -output out2\output3_06s.png 
..\bin\four_x64_Release -input scenes2\scene3_07_plane.txt -size 200 200 -output out2\output3_07s.png
..\bin\four_x64_Release -input scenes2\scene3_08_sphere.txt -size 200 200 -output out2\output3_08s.png  
..\bin\four_x64_Release -input scenes2\scene3_09_exponent_variations.txt -size 300 300 -output out2\output3_09s.png 
..\bin\four_x64_Release -input scenes2\scene3_10_exponent_variations_back.txt -size 300 300 -output out2\output3_10s.png 
..\bin\four_x64_Release -input scenes2\scene3_11_weird_lighting_diffuse.txt -size 200 200 -output out2\output3_11s.png 
..\bin\four_x64_Release -input scenes2\scene3_12_weird_lighting_specular.txt -size 200 200 -output out2\output3_12s.png

..\bin\four_x64_Release -input scenes2\scene4_01_sphere_shadow.txt -size 200 200 -output out2\output4_01s.png -shadows 
..\bin\four_x64_Release -input scenes2\scene4_02_colored_shadows.txt -size 200 200 -output out2\output4_02s.png -shadows
REM transform
REM ..\bin\four_x64_Release -input scenes2\scene4_03_mirrored_floor.txt -size 200 200 -output out2\output4_03s.png -shadows -bounces 1 -weight 0.01 

..\bin\four_x64_Release -input scenes2\scene4_04_reflective_sphere.txt -size 200 200 -output out2\output4_04as.png -shadows -bounces 0 -weight 0.01
..\bin\four_x64_Release -input scenes2\scene4_04_reflective_sphere.txt -size 200 200 -output out2\output4_04bs.png -shadows -bounces 1 -weight 0.01
..\bin\four_x64_Release -input scenes2\scene4_04_reflective_sphere.txt -size 200 200 -output out2\output4_04cs.png -shadows -bounces 2 -weight 0.01
..\bin\four_x64_Release -input scenes2\scene4_04_reflective_sphere.txt -size 200 200 -output out2\output4_04ds.png -shadows -bounces 3 -weight 0.01

REM transform
REM ..\bin\four_x64_Release -input scenes2\scene4_10_point_light_distance.txt -size 200 200 -output out2\output4_10s.png -shadows 

REM requires transform
REM ..\bin\four_x64_Release -input scenes2\scene4_11_point_light_circle.txt -size 200 200 -output out2\output4_11s.png -shadows 
REM ..\bin\four_x64_Release -input scenes2\scene4_12_point_light_circle_d_attenuation.txt -size 200 200 -output out2\output4_12s.png -shadows 
REM ..\bin\four_x64_Release -input scenes2\scene4_13_point_light_circle_d2_attenuation.txt -size 200 200 -output out2\output4_13s.png -shadows 

REM ..\bin\four_x64_Release -input scenes2\scene6_13_checkerboard.txt -size 200 200 -output out2\output6_13s.png -shadows 

REM anti aliasing
..\bin\four_x64_Release -input scenes2/scene7_01_sphere_triangle.txt -size 200 200 -output out2\output7_01.png 
..\bin\four_x64_Release -input scenes2/scene7_01_sphere_triangle.txt -size 9 9 -output out2\output7_01_nosampling.png 
..\bin\four_x64_Release -input scenes2/scene7_01_sphere_triangle.txt -size 9 9 -output out2\output7_01_uniform_9.png -uniform_samples 9
..\bin\four_x64_Release -input scenes2/scene7_01_sphere_triangle.txt -size 9 9 -output out2\output7_01_regular_9.png -regular_samples 9
..\bin\four_x64_Release -input scenes2/scene7_01_sphere_triangle.txt -size 9 9 -output out2\output7_01_jittered_9.png -jittered_samples 9

REM This is for extra credit
REM ..\bin\four_x64_Release -input scenes2\scene6_14_glass_sphere.txt -size 200 200 -output out2\output6_14s.png -bounces 4 -shadows 
