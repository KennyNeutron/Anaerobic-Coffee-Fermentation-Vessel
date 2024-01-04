//Display Manager
void show_string(String str, int16_t x, int16_t y, uint8_t csize, uint16_t fc, uint16_t bc, boolean mode) {
  tft.Set_Text_Mode(mode);
  tft.Set_Text_Size(csize);
  tft.Set_Text_colour(fc);
  tft.Set_Text_Back_colour(bc);
  tft.Print_String(str, x, y);
}

void disp_LoadingScreen() {
  tft.Fill_Screen(BLACK);
  show_string("WELCOME TO", CENTER, 150, 6, WHITE, WHITE, 0);
  show_string("Anaerobic Coffee Fermentation Vessel Monitor", CENTER, 250, 3, WHITE, WHITE, 0);
  show_string("STARTING...", CENTER, 450, 3, WHITE, WHITE, 0);
}