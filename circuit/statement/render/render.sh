pdflatex main.tex
convert -density 300 -background white -alpha remove -alpha off main.pdf main.png
mv main-0.png ../circuit-example.png
mv main-1.png ../circuit-example-assignment.png
mv main-2.png ../circuit-sample1-update1-way1.png
mv main-3.png ../circuit-sample1-update1-way2.png
cd ..
mogrify -resize 225x circuit-example.png
mogrify -resize 225x circuit-example-assignment.png
mogrify -resize 200x circuit-sample1-update1-way1.png
mogrify -resize 200x circuit-sample1-update1-way2.png
