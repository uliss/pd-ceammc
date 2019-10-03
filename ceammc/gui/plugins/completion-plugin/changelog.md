# Changelog for Pure Data auto completion tcl plugin

## v0.47.1 (Henri Augusto)

### Bug Fixes

  * fixes a bug where the plugin would not save/read the configs from the right folder if the user had any plugin load after the completion-plugin (it was reading "$::current_plugin_loadpath")

### Improvements

  * Now Control+Shift+Up/Down are used to skip 10 rows up/down (consistent with Ctrl+Left/Right)

## v0.47.0 (Henri Augusto)

### Bug Fixes

  * Now that you can set custom-hotkeys this exposed an issue where the keys being used for trigger the plugin would also be typed in the object box. That was fixed by waiting 200ms without processing any KeyRelease event (which should be enough for the user to release the keys used to trigger the plugin)
  * users with kbds with languages where "~" is a MultiKey: you can type ~ while seeing the suggestions window by pressing ~ twice (just like you could type ~ in PD <=0.49)
    * ex: Portuguese, French, spanish, etc. I've only tested it with my pt-br keyboard, though.
  * added some support for typing more chars in ::completion::lb_keyrelease
  * fixed bug on parcial completions (completing a common prefix of all suggestions)

### improvements

  * improvements for the object box deletion when using a special message (options,  rescan, debug, etc)
  * general cleanup of code


## v0.47.0-test1 (Henri Augusto)

### new features

  * Changes related to pd 0.50
      * in this version the Tab key switches the selected object so now we bind the completion window to **Ctrl+SPace**
        * aditionally you can change the binding in the configurations
      * [pdcontrol] and [slop~] added to the vanilla object list
    -now you can use the suggestion window to access functionality
     * **completion::options**: open the options windows
     * **completion::rescan**: rescan the externals
     * **completion::help**: open the help patch
     * **completion::debug**: enter and exit debug mode
  * _the plugin automatically deletes the leftover object_
    * that might be optional in the future
    * it deletes the object by simulating user input. It is a **hack** so it will delete any object under the one you're typing

### Bug fixes

    -[namecanvas] suggestion, which was missing, was added

## v0.46.1 (Henri Augusto)

### Bug fixes

* fixes a bug where the plugin tried to bind to _.pop_ after auto completing when **::completion::unique** was true.

## v0.46.0 (Henri Augusto)

### new features

* added help button on settings that opens the help patch.

### Bug fixes

* popup is now destroyed when it loses keyboard focus
* now the plugin won't be triggered when the user is typing on comments
* complying with pd 0.49 now we add our setting menu to file->preferences instead of the duplicated (and now gone) edit->preferences
* fixed a bug where scrolling with the mouse wheel was calling choose_selected. Fixed by checking `%b eq 1` inside the <ButtonRelease> event!


## v0.45.0 (Henri Augusto)

### new features
* now you can use shift+enter when **auto complete library names** is *false* to complete the library name.
* added a rescan button to the settings window
* now the plugin detect which multi-object ('monolithic') libraries are actually installed before loading them into the completions
* now the plugin has a readme.pd file that deken can open at startup

### Bug fixes

* Fixed a bug where you would get an error when trying to save the settings


## v0.44.0 (Henri Augusto)

* *Major* overhaul and bugfix to the code to bring it up to working condition in par with pd 0.48 API.
* Now the plugin seems to be working on Windows, Linux and OSX!

### new features

* now you can configure the plugin under preferences->PD AutoComplete Settings
* Now *::completion::read_config* ignores any line that doesn't start with a char. 
* While reading the cfg file it reads lists of arguments instead of just one. Pprevents reading "DejaVu" instead of "DejaVu Sans Mono", for example.
* Now the plugin is able to run witout the .cfg file. It creates the .cfg file in it's absence
* I've implemented three search modes
    * **normal:** search for exact matches
    * **skip:** search for anything containing the input chars in order. Ex: plf matches zexy/[p]o[l]y[f]un
    * **monolithic:** search for objects contained in monolithic distributions (.dll, pd_darwin, .pdlinux). Those need to be declared in monolithicLibs.txt.
* Each search mode has a color
* The plugin intelligently scans the paths set by the user (edit->preferences->path) to scan for externals without the need for the user to type their name on a file. Consequently the script doesn't need a list of objects. 
* Yet you can still define useful stuff in any .txt inside the folder *custom_completions*. I've already added some useful keywords like 'anything', 'adddollar', etc and even some constants like Pi and the golden ratio.
* Added navigating faster with shift+up/down
* added warping the listbox ends (overriding the bindings)
* If the completions window would be drawed off-screen we adjust it's position so it is enterily visible.
* I'm disabling the unique names completion for now because i don't think it is desireable. While it does detects when the user type a new name it **doesn't** when those names are not used any more (user closed their containing patch, deleted their objects, etc). In future versions we should be able to do that communicating with PD directly.
* Flexible debugging options (see development guide.md)



## Yvan Volochine version history: 

![original project on GitHub](https://github.com/gusano/

## v0.43

 - new BSD License


## 0.42:

 - add `user_objects` file support
 - add optional offset for popup position
 - add forgotten drawpolygon

## 0.41:

 - cleanup, simplify focus behavior, remove unused proc, better bindings
 - add support to remember `send, receive, table, delread, ...` argument names
 - add libraries objects lists (Gem, gridflow, py)
 - various fixes

## 0.40:

 - new GUI
 - rename to 'completion-plugin.tcl'
 - add bash completion mode
 - add support for osx and win32
 - add *.cfg file for user options
 - TODO add support for user arguments (like [tabread foo], etc) ??

## 0.33:

 - cosmetic fixes for osx
 - better box coordinates
 - bugfix: popup menu wrongly placed with huge fonts

## 0.32:

 - add colors
 - bugfix: cycling has 1 step too much
 - bugfix: first completed doesn't erase typed text

## 0.31:

 - add TAB support to cycle through completions

## 0.3:

 - simplify cycling code
 - bugfix: nameclash with right-click popup (sic)
 - bugfix: missing or mispelled internals

## 0.2:

 - add popup menu for completion

## 0.12:

 - fix namespace