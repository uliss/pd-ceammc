#!/bin/bash

sleep $1

osascript <<EOD
   tell application "Wish"
       activate
   end tell
EOD
