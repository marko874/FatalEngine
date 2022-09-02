import os

os.system("glslc -fshader-stage=vert Default.vert.glsl -o Default.vert.spv")
os.system("glslc -fshader-stage=frag Default.frag.glsl -o Default.frag.spv")