pdflatex main.tex
convert -density 300 -background white -alpha remove -alpha off main.pdf main.png
mv main-0.png ../islands-sample1.png
mv main-1.png ../islands-sample2.png
cd ..
mogrify -resize 300x islands-sample1.png
mogrify -resize 300x islands-sample2.png
