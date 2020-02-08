# PD AutoComplete Plugin

This Gui-Plugin enables auto-completion for [pure-data](http://puredata.info) objects. 

* **Does it run on Vanilla?** Yes. It is a Tk/Tcl pluging made to run in Vanilla.
* **What about Purr Data?** Purr Data uses [nw.js](https://nwjs.io/) for it's GUI. So this pluging doesn't work in Purr Data. But if you're on windows you can use [PD AutoComplete Script](https://github.com/HenriAugusto/PD-AutoComplete-Script)

Here is a link to the [original repo by Yvan Volochine](https://github.com/gusano/completion-plugin).

## Gif

![PD AutoComplete Plugin gif](https://github.com/HenriAugusto/completion-plugin/blob/master/images/PD_completion-plugin_gif_demo.gif)

## How to install:

*After* you install the plugin **you must restart Pure Data**.

### Deken (easy way)

 - just search for the completion-plugin on deken <3
 - **you need at least Deken 0.3.1** to download the completion plugin. **Pd-0.48-1 comes with deken-0.2.4**! So either
    - download Pd-0.48-2
    - open Deken on PD 0.48-1 or older and search for deken. Then install the newer Deken version through Deken.

### Manually (not so easy way)

 - just [download](https://github.com/HenriAugusto/completion-plugin/releases) the plugin and put the whole `PD-AutoComplete-plugin` folder anywhere and add it to your pd paths in **file->Preferences->Path**
 - The easiest way is it on the "extras" folders of your pd install. If you do this you don't need to set the path.
 - Yet i recommend having a "shared extras" folder and add it to your PD Path. This way if you have more than 1 pd installs (example: if you've used the zip distributions to have more than one PD version)


## How to use:

Just hit the TAB key while typing into an object to trigger completion mode.

Use up and down to move through the suggestions. Use shift+arrows for faster navigation.

### Search modes

There are three search modes

* **normal:** search for exact matches
* **skip:** search for anything containing the input chars in order. Ex: plf matches zexy/[p]o[l]y[f]un
* **monolithic:** search for objects contained in **multi-object** ("monolithic") distributions (.dll, pd_darwin, .pdlinux). Those are read from monolithicLibs.txt. The respective library must be loaded! See [this link](https://github.com/pure-data/externals-howto#library) on that matter.

How to use each one:

* Just type normally to use **normal**
* Start your search with an "**.**" to use **skip**
* Start your search with an "**,**" to use **monolithic**

### Externals scanning

The plugin intelligently scans the paths set by the user (file->preferences->path) to scan for externals without the need for the user to type their name on a file. Consequently the script doesn't need a list of objects. 

It searches the static default paths (ex: *C:/PureData/pd-0.48-0.msw/pd/extra/*) for libraries and then searches any path you've set in *file->preferences->path* or that Deken have set for you.

#### duplicates

Some objects **by design** might be scanned twice as this reflects Pure Data objection instantiation.

If you've ser for example the following folder in *file->preferences->path*

```
C:/Users/Stravinsky/Dropbox/pd-0.48-0.msw/pd/extra/iemguts/
```

You can use the canvasargs external in two ways (regardless of the autocomplete plugin):

[canvasargs]
[iemguts/canvasargs]

The first use the path you've set. The second uses the standard path. So the autocomplete plugin will show two options for canvasargs.
Notice that the first method doesn't avoid naming conflicts while the seconds does. For that reason the latter is usually preferred.

#### Extra keywords

You can define useful stuff in any .txt inside the folder *custom_completions*. I've already added some useful keywords like "anything", "adddollar", etc and even some constants like Pi and the golden ratio.

### Settings

* now you can configure the plugin under preferences->PD AutoComplete Settings.

 - **auto complete library names:**
   - *on:* [list-abs/list-clip]
   - *off:* [list-clip]
   - *When the "autocomplete libraries" option is *enabled*: you can use shift+enter to type obj with the library*
   - *When the "autocomplete libraries" option is *disabled*: you can use shift+enter to type only the object name (withouh the library)*
- **Number of lines to display:** number of completion suggestions the plugin will display
- **Font size:** the size of the font used for the suggestions window
- **Maximum scan depth:** how deep the plugin will look inside a search path.
   - *Example:* the *iemguts* libraries's folder contains a subfolder "example" with some explanatory patches that you might not want to scan.
   - So if you set a path to the iemguts folder and use a value of 1 for max scan depth it will only scan the stuff inside the *iemguts/* folder. If you set 2 or more it will also scan *iemguts/examples*
- **bkg color options:** change the background color for each search mode.

- **rescan:** update the completions after you installed/uninstalled externals.
- **default:** reset the plugin settings to the default
- **save to file:** save your settings to the HD. The settings are saved in the `completion.cfg` config file. 
   - Even if you change the pluging settings the new settings won't be remembered between pd sessions unless you save them to the file.

Settings are applied immediately after you change them but are only saved when you click "save to file". That means, unless you save them to the file, the next time you run PD the plugin will use the previous settings.


### Development 

Please fill in an issue on the github repository if you find a bug.

#### Development Guide

I've written a [developtment guide](https://github.com/HenriAugusto/completion-plugin/blob/master/development%20guide.md) to make it easier to tackle on the code.

#### Change log

You can find it [here.](https://github.com/HenriAugusto/completion-plugin/blob/master/changelog.md)
