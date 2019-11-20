set palette rgbformula 21,22,23
set cbrange [500:1200]
set cblabel "Temperature"
unset cbtics
set view map
splot "out.txt" matrix with image
