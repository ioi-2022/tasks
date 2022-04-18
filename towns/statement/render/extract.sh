pdflatex main.tex
convert -density 300 -background white -alpha remove -alpha off main.pdf main.png
mv main-0.png ../towns-1.png
mv main-1.png ../towns-2.png
cd ..
mogrify -resize 300x towns-1.png
mogrify -resize 300x towns-2.png
