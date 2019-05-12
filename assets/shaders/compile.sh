#!/usr/bin/env sh

for d in */; do
    echo "Compiling shaders in ${d}"
    cd ${d}
    echo "Compiled ${d}$(glslangValidator -V *.vert)"
    echo "Compiled ${d}$(glslangValidator -V *.frag)"
    cd ..
done