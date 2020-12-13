#
# Makefile for all magic stuff inside the report
#  @author Rafael Lima
#

################################################################################
# Variables 
################################################################################

# Programs ---------------------------------------------------------------------

DOT = dot
OCTAVE = octave
MATLAB = matlab -nodesktop -nosplash
TEX = pdflatex
TEXFLAGS = --enable-write18 --shell-escape

# Directories ------------------------------------------------------------------

LATEXDIR = src/tex
FIGDIR = src/tex/img
DOTDIR = src/dot
MATLABDIR = src/matlab

# Target Files -----------------------------------------------------------------

# Dot stuff
DOTFILES = $(wildcard $(DOTDIR)/*.dot) $(DOTDIR)/petrinet_tree.dot
PICFILES = $(addprefix $(FIGDIR)/,$(notdir $(DOTFILES:%.dot=%.png)))

# LaTeX
TEXSRC = $(wildcard $(LATEXDIR)/project.tex)
PDFOUTPUT = $(TEXSRC:.tex=.pdf)

###############################################################################
# Rules 
###############################################################################

# One rule to rule them all
.PHONY: all
all: $(PICFILES)

# Print help for this Makefile commands
.PHONY: help
help:
	@echo "Use: make -f Makefile [OPTION]"
	@echo "OPTIONS"
	@sed Makefile -n -e "N;s/^# \(.*\)\n.PHONY:\(.*\)/ \2:\1/p;D" | column -ts:
	@echo ""

# Remove Generated Files
.PHONY: clean
clean:
	rm -vf $(PICFILES)

# Images -----------------------------------------------------------------------

# Dot figures
$(FIGDIR)/%.png: $(DOTDIR)/%.dot
	$(DOT) -Tpng $^ -o $@

# Simulation Scripts -----------------------------------------------------------

$(DOTDIR)/petrinet_tree.dot: $(MATLABDIR)/petrinetproject.m
	cd $(dir $<) && $(MATLAB) -sd $(dir $<) -r "$(basename $(notdir $<)); quit"

# LaTeX Reports ---------------------------------------------------------------

# Generate Report
.PHONY: report
report: $(PDFOUTPUT)

# Implicity Rule to Compile texfile
$(LATEXDIR)/%.pdf: $(LATEXDIR)/%.tex | $(PICFILES) 
	cd $(dir $<) && $(TEX) $(TEXFLAGS) $(notdir $<)
	cd $(dir $<) && bibtex $(basename $(notdir $<))
	cd $(dir $<) && $(TEX) $(TEXFLAGS) $(notdir $<)

# Remove LaTeX Temporary files
.PHONY: clean-tex
clean-tex:
	rm -fv src/tex/*.aux src/tex/*.bbl src/tex/*.blg src/tex/*.log src/tex/*.idx src/tex/*.out src/tex/*.nav src/tex/*.snm src/tex/*.toc src/tex/*.vrb

# Remove Generated PDF Files
.PHONY = clean-pdf
clean-pdf: clean-tex
	rm -fv ${PDFOUTPUT}



