LANG=en

install-data-local:
	$(mkinstalldirs) $(kde_htmldir)/${LANG}/bibletime; \
 	for file in *.html; do \
		$(INSTALL_DATA) $$file $(kde_htmldir)/${LANG}/bibletime && \
		chmod a+r $(kde_htmldir)/${LANG}/bibletime/$$file; \
	done; \
	cp -Ra common $(kde_htmldir)/${LANG}/bibletime; \
	cd ..; \
	chmod a+r+X -R $(kde_htmldir)/${LANG}/bibletime/;

uninstall-local:
	@for f in *.html; do \
		echo Removing $(kde_htmldir)/${LANG}/bibletime/$$f \
		-rm -f $(kde_htmldir)/${LANG}/bibletime/$$f \
	done; \
	cd .. \
	echo Deleting $(kde_htmldir)/${LANG}/bibletime/common;
	rm -Rf $(kde_htmldir)/${LANG}/bibletime/common;

EXTRA_DIST = h2_basics.html h2_basics_approaches.html h2_basics_expository.html h2_basics_interpretation.html h2_basics_types.html h2_basics_worksheet.html h2_idx.html h2_importance.html h2_importance_breathed.html h2_importance_exhortations.html h2_importance_liberates.html h2_importance_once.html h2_importance_supplement.html h2_importance_wars.html h2_importance_works.html h2_rules.html h2_rules_context.html h2_rules_hcontest.html h2_rules_normal.html h2_rules_parables.html hdbk_bars.html hdbk_bars_menubar.html hdbk_idx.html hdbk_intro.html hdbk_intro_motivation.html hdbk_intro_todo.html hdbk_op.html hdbk_op_mdi_window.html hdbk_op_module_tree_select.html hdbk_op_output.html hdbk_op_search.html hdbk_start_screen.html hdbk_term.html hdbk_term_hotkeys.html help_first_startup.html help_first_time_search.html help_idx.html help_initbackend_failed.html help_module_encrypted.html help_no_mods_d.html help_no_modules.html index.html inst_bin.html inst_devl.html inst_devl_cvs.html inst_idx.html inst_preq.html inst_preq_install.html inst_sources.html inst_sources_bibletime.html inst_sources_compiler_notes.html inst_user.html


