set palette rgbformula 2,-7,-7
set cbrange [20:26]
set cblabel "Temperature"
unset cbtics
set view map
splot "out.txt" matrix with image
