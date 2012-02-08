Ian Dunn
idunn01@calpoly.edu

Run make from root directory to compile all libraries and application.
./MeshLoaderDemo/MeshLoaderDemo is the resulting program.

Controls are WASD to move camera, 1-3 to change materials, E for Scale mode, R for Rotate mode, T for Translate mode.
F to use flat shading, V to use smooth shading
Z to use vertex-lighting shader, X to use per-pixel lighting shader without specular, C to use per-pixel lighting shader with specular

Scene contains 3 lights surrounding the bunny, one red one green and one blue

Also note that my mult-light specular shader failed to compile on my laptop, while successfully building
on my desktop. I'm hoping the OpenGl 4.0+ lab computers will compile it. If not, pressing X will simply hide the bunny.
