pdflatex main.tex
convert -density 300 -background white -alpha remove -alpha off main.pdf main.png
mv main-0.png ../fish-sample1-pond.png
mv main-1.png ../fish-sample1-pier.png
cd ..
mogrify -resize 300x fish-sample1-pond.png
mogrify -resize 300x fish-sample1-pier.png
