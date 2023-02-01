declare name "flt.dcblock2";

fi = library("filters.lib");
cm = library("ceammc.lib");

process = cm.duplicate(2, fi.dcblocker);
