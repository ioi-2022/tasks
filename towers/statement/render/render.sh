pdflatex main.tex
convert -density 300 -background white -alpha remove -alpha off main.pdf main.png
mv main.png ../towers-example.png
cd ..
mogrify -resize 400x towers-example.png
