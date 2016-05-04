all: s3tp.pdf

s3tp.pdf: s3tp.tex s3tp-overview.pdf
	pdflatex s3tp

s3tp-overview.pdf: s3tp-overview.ipe
	ipetoipe -pdf $< $@

clean:
	rm -f *.pdf *.aux *.log *.out

.PHONY: clean
