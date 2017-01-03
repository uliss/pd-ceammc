#!/bin/bash
# This file must be saved in UTF-8


################################
# Uninstallations Methodes
################################
function remove5 {
	echo "Removing RevcLibrary for Max6";
	doRemove;
}

function doRemove {
	echo -ne "- RevLibrary Installer Receipt\n"
	sudo rm -Rf "/private/var/db/receipts"/com.cicm.revlibrary.*
	echo -ne "... done.\n"
	
	if [ -e "$C74Folder" ] ; then
		echo -ne "- RevLibrary Folder ($C74Folder/RevLibrary/)"
		sudo rm -Rf "$C74Folder/RevLibrary" && echo -ne "... done.\n" 
	fi
	

	if [ -e "$maxAppFolder" ] ; then
		
		echo -ne "- Extra Files ($maxAppFolder/patches/extras/rev.overview.maxpat)"
		sudo rm -Rf "$maxAppFolder/patches/extras/rev.overview.maxpat" && echo -ne "... done.\n"
	fi
 
	echo -ne "\n";

}


################################
# UnInstallation process
################################
clear

echo "------------------------------"
echo "--- RevLibrary Uninstaller ---"
echo "------------------------------"
echo ""
echo "Hi $USER..."
echo "You must run this script as an Admin user, and may be prompted for your password."
echo ""


################################
#  Version checking
################################
whichVersion=0;

maxAppFolder="/Applications/Max6";
C74Folder="$maxAppFolder/Cycling '74"; 
remove5;

echo -ne "done"
echo -ne "\n\nend of RevLibrary Uninstaller.\n"
echo -ne "(you can quit the Terminal now...)\n"

exit 0;
