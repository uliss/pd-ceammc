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
            $mymenu entryconfigure [_ "CEAM"] -command {::ceammc_prefs::show_dialog}
        } _]
    } {
        $mymenu add separator
        $mymenu add command -label [_ "CEAM"] -command {::ceammc_prefs::show_dialog}
    }
}

proc ::ceammc_prefs::show_dialog {} {
    if {[winfo exists .ceammc_prefs]} {
        focus .ceammc_prefs
        return
    }

    toplevel .ceammc_prefs -background $::pd_colors::window_background
    wm title .ceammc_prefs [_ "ceammc external settings"]
    wm minsize .ceammc_prefs 250 50

    # close on escape press
    bind .ceammc_prefs <Escape> {destroy .ceammc_prefs}
    # close by ⌘+w (only for macOS)
    if {$::tcl_platform(os) eq "Darwin"} {
        bind .ceammc_prefs <Command-w> {destroy .ceammc_prefs}
    }

    ttk::frame .ceammc_prefs.f -padding 10
    ttk::label .ceammc_prefs.f.lang_label -text [_ "Language:"]

    ttk::combobox .ceammc_prefs.f.lang \
        -values [list [_ "Default"] [_ "English"] [_ "Russian"]] \
        -state readonly

    bind .ceammc_prefs.f.lang <<ComboboxSelected>> {
        switch [%W current] {
            0 { pdsend "; ceammc doc default\n" }
            1 { pdsend "; ceammc doc en\n"      }
            2 { pdsend "; ceammc doc ru\n"      }
        }
    }
    .ceammc_prefs.f.lang current $::ceammc_prefs::lang

    # setup main frame stuff
    grid .ceammc_prefs.f -column 0 -row 0

    # show
    set padding 5
    grid .ceammc_prefs.f.lang_label -column 0 -row 0 -padx $padding -pady $padding -sticky "w"
    grid .ceammc_prefs.f.lang -column 1 -row 0 -padx $padding -pady $padding -sticky "w"
}

::ceammc_prefs::init_options_menu
::pdwindow::debug "\[ceammc\] ceammc_settings-plugin loaded\n"
