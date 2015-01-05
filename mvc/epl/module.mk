# @file module.mk
#
local_dir := $(build_dir)/$(sub_dir)
local_sources := \
	$(addprefix $(sub_dir)/,epl_parse.y epl_scan.l) \
	$(wildcard $(sub_dir)/*.cpp)
local_LFLAGS := --header-file=$(local_dir)/epl_scan.hpp
local_CXXFLAGS := -I$(local_dir)

$(eval $(call make-library,$(sub_dir)/libmvepl.a,$(local_sources)))

$(eval $(call compile-rules,CPP,$(local_CXXFLAGS)))

.SECONDARY: $(call generated-sources, $(local_sources))

$(local_dir)/epl_scan.d: $(local_dir)/epl_parse.d

$(local_dir)/%.cpp $(local_dir)/%.h: $(sub_dir)/%.y
	@echo + yacc $<
	$(V)$(YACC) $(YFLAGS) --output=$(subst .h,.cpp,$@) $<

$(local_dir)/%.cpp: $(sub_dir)/%.l
	@$(RM) $@
	@echo + lex $<
	$(V)$(LEX) $(LFLAGS) $(local_LFLAGS) --outfile=$@ $<
