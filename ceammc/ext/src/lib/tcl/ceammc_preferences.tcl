namespace eval ::ceammc_prefs:: {
    variable lang ;# current language
}

# keep in sync with: enum class ceammc_config_lang
# 0 - Default
# 1 - English
# 2 - Russian
set ::ceammc_prefs::lang 0

# note: this name is used in ceammc external!
proc ::ceammc_prefs::set_doc_lang {lang_idx} {
    set ::ceammc_prefs::lang $lang_idx
}

proc ::ceammc_prefs::request_doc_lang {} {
    pdsend "; ceammc get doc_lang {::ceammc_prefs::set_doc_lang}\n"
}

proc ::ceammc_prefs::init_options_menu {} {
    if {$::windowingsystem eq "aqua"} {
        set mymenu .menubar.apple.preferences
    } else {
        set mymenu .menubar.file.preferences
    }

    if {
        [catch {
            $mymenu entryconfigure [_ "ceammc"] -command {::ceammc_prefs::show_dialog}
        } _]
    } {
        $mymenu add separator
        $mymenu add command -label [_ "ceammc"] -command {::ceammc_prefs::show_dialog}
    }
}

proc ::ceammc_prefs::show_dialog {} {
    if {[winfo exists .options]} {
        focus .options
        return
    }

    toplevel .options -background $::pd_colors::window_background
    wm title .options [_ "ceammc external settings"]

    ttk::frame .options.f -padding 10
    ttk::label .options.f.lang_label -text [_ "Language:"]

    ttk::combobox .options.f.lang \
        -values [list [_ "Default"] [_ "English"] [_ "Russian"]] \
        -state readonly

    bind .options.f.lang <<ComboboxSelected>> {
        switch [%W current] {
            0 { pdsend "; ceammc doc default\n" }
            1 { pdsend "; ceammc doc en\n"      }
            2 { pdsend "; ceammc doc ru\n"      }
        }
    }
    .options.f.lang current $::ceammc_prefs::lang

    # setup main frame stuff
    grid .options.f -column 0 -row 0

    # show
    set padding 5
    grid .options.f.lang_label -column 0 -row 0 -padx $padding -pady $padding -sticky "w"
    grid .options.f.lang -column 1 -row 0 -padx $padding -pady $padding -sticky "w"
}

::ceammc_prefs::init_options_menu
::pdwindow::debug "\[ceammc\] ceammc_settings-plugin loaded\n"
