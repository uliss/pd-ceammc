#include "mod_path.h"

void setup_file_size();
void setup_path_basename();
void setup_path_dirname();
void setup_path_exists();
void setup_path_is_dir();
void setup_path_lsdir();
void setup_path_monitor();
void setup_path_normalize();
void setup_path_pattern();
void setup_path_search();
void setup_path_split();

void ceammc_path_setup()
{
    setup_file_size();
    setup_path_basename();
    setup_path_dirname();
    setup_path_exists();
    setup_path_is_dir();
    setup_path_lsdir();
    setup_path_monitor();
    setup_path_normalize();
    setup_path_pattern();
    setup_path_search();
    setup_path_split();
}
