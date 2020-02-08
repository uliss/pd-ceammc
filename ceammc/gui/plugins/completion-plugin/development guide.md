## completion-plugin development guide

The original version was developed by Yvan Volochine a long ago (as the time of writing: 7 years ago). I then embraced the project and i'm willing to mantain it for as long as i can.
I will be hosting it on [github](https://github.com/HenriAugusto/completion-plugin)

I wrote this as a guide to make easier to people to colaborate on the completion-plugin. Specially if they're new to TCL (as i was when i took the project) that might help a lot.

## TCL

First of all i wanna point that this one is the most newbie-friendly among the TCL documentation: [TCL Tutorial](http://www.tcl.tk/man/tcl8.5/tutorial/tcltutorial.html)

Also don't miss this one specific to Pure Data [GUIPlugins](https://puredata.info/docs/guiplugins/GUIPlugins/)

### Some important things i will break down here

* **whitespaces are important and can break your code!**
* Ones does not simply write math. You must use 'expr'

## How it works

The pluging overwriting the *pdtk_text_editing* proc located in pdtk_text.tcl in the PD sources. This it is able to "plug" into puredata and set important variables like

* ::current_canvas - the canvas where the user is typing
* rectcoords - thee coordinates of the object where he/she is typing

Inside *::completion::popup_draw* we create the listbox (that window with the code suggestions) and add event listeners (called *bindings* in TCL) so we are able to process user input.

We then store userinput into **::current_text** and later use it perform searches among the stored object names. They're into *::all_externals*. It contains a hardcoded list of vanilla objects. Also on startup we call *::completion::add_user_externals* which scans the folders and subfolders (recursively) of all the external paths that the user have set into "edit->preferences->path" or were added by Deken.

I've added ::completions::debug_msg for an toggable debug function that prints to the PD console. It is controled by the variable **::completion_debug** that should be false (or 0) in final releases but can be set to true (or 1) while developing.

## debugging

I've added a *::completion::debug_msg* method that can be used to post messages to the pd console with a "autocmpl_dbg:" prefix.

```tcl
::completion::debug_msg "this message will be posted to the pd console"
```

I've also added a variable for toggling debug mode on/off

```tcl
set ::completion_debug 1 ;#1 = true 0 = false
```

Furthermore sometimes you may only want to debug a single aspect of the plugin. So for that i've added specific variables that are used to set that kind of thinks you want to debug

```tcl
set ::completion_debug 1 ;
set ::debug_loaded_externals 0 ;#prints loaded externals
set ::debug_entering_procs 0 ;#prints a message when entering a proc
set ::debug_key_event 0 ;#prints a message when a key event is processed
set ::debug_searches 0 ;#messages about the performed searches
set ::debug_popup_gui 0 ;#messages related to the popup containing the code suggestions
set ::debug_char_manipulation 0 ;#messages related to what we are doing with the text on the obj boxes (inserting/deleting chars)
```

This way you can add a debug message and set it's "tag" so you can control when to debug it

```tcl
# debug variables on the beggining of the code
set ::debug_loaded_externals 1 ;#prints loaded externals
set ::debug_key_event 0 ;#prints a message when a key event is processed
# some calls on the middle of the code
::completion::debug_msg "this message will be posted" "loaded_externals"
::completion::debug_msg "this will not" "key_event"
```

*(notice you don't include ::debug_ on the tags!)*

Of course if *::completion_debug * is set to 0 (or false) no debug message will be posted regardless of the specific debug configuration

## Some minor comments on the changes i've made to Yvan's code

1. I'm disabling the unique names completion for now because i don't think it is desireable. While it does detects when the user type a new name it **doesn't** when those names are not used any more (user closed their containing patch, deleted their objects, etc). In future versions we should be able to do that communicating with PD directly.
1. There seems to be a bug on the ListBox widget. 
    * you can't override the <Next> and <Prior> keys (even if you remove all bindtags except for .pop.f.lb)

### Useful links

[List of keysyms you can use to bind keyevents](https://www.tcl.tk/man/tcl8.5/TkCmd/keysyms.htm)
