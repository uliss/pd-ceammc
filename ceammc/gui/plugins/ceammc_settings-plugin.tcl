# ceammc external settings plugin
package require pd_guiprefs

namespace eval ::ceammc_prefs:: {
    variable lang ;# current language
}

set ::ceammc_prefs::lang 0

proc ::ceammc_prefs::set_doc_lang {lang_idx} {
    ::pdwindow::debug "set lang ${lang_idx}\n"
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
            0 { pdsend "; ceammc doc default\n"; ::ceammc_prefs::set_doc_lang 0; }
            1 { pdsend "; ceammc doc en\n";      ::ceammc_prefs::set_doc_lang 1; }
            2 { pdsend "; ceammc doc ru\n";      ::ceammc_prefs::set_doc_lang 2; }
        }
    }

    # setup main frame stuff
    grid .options.f -column 0 -row 0

    # show
    set padding 5
    grid .options.f.lang_label -column 0 -row 0 -padx $padding -pady $padding -sticky "w"
    grid .options.f.lang -column 1 -row 0 -padx $padding -pady $padding -sticky "w"

    ::pdwindow::debug "\[ceammc\]: ceammc_settings-plugin loaded\n"
}

::ceammc_prefs::init_options_menu

# request lang from loaded ceammc
after 5000 {::ceammc_prefs::request_doc_lang}
