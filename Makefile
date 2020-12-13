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

# Directories ------------------------------------------------------------------

FIGDIR = src/tex/img
DOTDIR = src/dot
MATLABDIR = src/matlab

# Target Files -----------------------------------------------------------------

# Dot stuff
DOTFILES = $(wildcard $(DOTDIR)/*.dot)
PICFILES = $(addprefix $(FIGDIR)/,$(notdir $(DOTFILES:%.dot=%.png)))

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

petrinetproject: $(MATLABDIR)/petrinetproject.m
	cd $(dir $<) && $(MATLAB) -sd $(dir $<) < $(notdir $<)

