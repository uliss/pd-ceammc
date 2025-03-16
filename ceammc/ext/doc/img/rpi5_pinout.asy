size(180); 

real R = 0.12;
real R0 = 0.05;
real dx0 = 0.4;
real dy0 = 0.3;
int N = 20;
defaultpen(fontsize(4pt));
texpreamble("\usepackage{cmbright}");

string[] GPIO1={"GPIO 21", "GPIO 20", "GPIO 16", "Ground", "GPIO 12", "Ground", "GPIO 01", "GPIO 07", "GPIO 08", "GPIO 25",
              "Ground", "GPIO 24", "GPIO 23", "Ground", "GPIO 18", "GPIO 15", "GPIO 14", "Ground", "5V", "5V" };

string[] GPIO0={"Ground", "GPIO 26", "GPIO 19", "GPIO 13", "GPIO 06", "GPIO 05", "GPIO 00", "Ground", "GPIO 11", "GPIO 09",
              "GPIO 10", "3.3V", "GPIO 22", "GPIO 27", "GPIO 17", "Ground", "GPIO 04", "GPIO 03", "GPIO 02", "3.3V" };

string[] INFO0={"", "", "(PCM\rule{.45em}{.2pt}FS)", "(PWM1)", "", "", "(ID\rule{.45em}{.2pt}SD)", "", "(SPI0\rule{.45em}{.2pt}CLK)", "(SPI0\rule{.45em}{.2pt}MISO)",
              "(SPI0\rule{.45em}{.2pt}MOSI)", "", "", "", "", "", "(GPCLK0)", "(SCL1,\,I\textsuperscript{2}C)", "(SDA1,\,I\textsuperscript{2}C)", "" };

string[] INFO1={"(PCM\rule{.45em}{.2pt}DOUT)", "(PCM\rule{.45em}{.2pt}DIN)", "", "", "(PWM0)", "", "(ID\rule{.45em}{.2pt}SC)", "(SPI\rule{.45em}{.2pt}CE1)", "(SPI\rule{.45em}{.2pt}CE0)", "",
              "", "", "", "", "(PWM0)", "(RXD0,\,UART)", "(TXD0,\,UART)", "", "", "" };

pen PG = heavygreen;
pen PM = heavymagenta;
pen PR = heavyred;
pen PCN = heavycyan;
pen[] PIN_COLOR0 = {black, PG, PG, PG, PG, PG, deepyellow, black, PM, PM, PM, PR, PG, PG, PG, black, PG, PCN, PCN, PR };

pen[] PIN_COLOR1 = {PG, PG, PG, black, PG, black, deepyellow, PM, PM, PG, black, PG, PG, black, PG, orange, orange, black, red, red };

// draw pin numbers
real LINE_L = -2.3;
real LINE_R = 2.7;
real NUM_L = -0.57;
real NUM_R = 0.96;
for (int n = 0; n < N; ++n) {
  real y = (n - 0.5) * dy0;
  draw((LINE_L, y) -- (LINE_R, y), grey + linewidth(0.2pt));
  //label(format("\textit{%02i}", 40 - (n*2)), (NUM_R, n*dy0), align=W, grey);
  //label(format("\textit{%02i}", 40 - (n*2)-1), (NUM_L, n*dy0), align=E, grey);
}

// draw pin number header
real y0 = (N - 0.5) * dy0;
draw((LINE_L, y0) -- (LINE_R, y0), grey + linewidth(0.2pt));
//label("\textit{Pin}", (NUM_R, N * dy0 - 0.05), align=W, grey);
//label("\textit{Pin}", (NUM_L, N * dy0 - 0.05), align=E, grey);

real y0 = (N - 0.5) * dy0;
draw((LINE_L, y0) -- (LINE_R, y0), grey + linewidth(0.2pt));
label("\textit{NAME}", (LINE_R, N * dy0 - 0.05), align=W, grey);
label("\textit{NAME}", (LINE_L, N * dy0 - 0.05), align=E, grey);


// draw pin name and INFO
real INFO_L = -1.7;
real INTO_R = 2.1;
for (int n = 0; n < N; ++n) {
  real y = (n - 0.5) * dy0;
  label("\textbf{" + GPIO1[n] + "}", (LINE_R, n*dy0), align=W, black);
  label("\textbf{" + GPIO0[n] + "}", (LINE_L, n*dy0), align=E, black);
  label(INFO0[n], (INFO_L, n*dy0 - 0.01), align=E, grey);
  label(INFO1[n], (INTO_R, n*dy0 - 0.01), align=W, grey);
}

// draw main rect
path p = box((-0.25,-dy0), (0.65, N * (dy0)));
fill(p, grey);
draw(p, black + linewidth(0.5pt));
  
path pin = circle((0,0), R);
path pin0 = circle((0,0), R0);

// draw left pins
for (int n = 0; n < N; ++n) {
  if (n == N-1) {
    path x = shift(0, n * dy0) * box((-R, -R), (R, R));
  	fill(x, PIN_COLOR0[n]);
  	draw(x, black + linewidth(0.25pt));
  } else {
  	path x = shift(0, n * dy0) * pin;
  	fill(x, PIN_COLOR0[n]);
  	draw(x, black + linewidth(0.25pt));
  }
  
  //path x0 = shift(0, n * dy0) * pin0;
  //fill(x0, white);
  //draw(x0, black + linewidth(0.25pt));
  
  label(format("%02i", 40 - (n*2)-1), (-0.125, n*dy0), align=E, white);
}

// draw right pins
for (int n = 0; n < N; ++n) {
  path x = shift(dx0, n * dy0) * pin;
  fill(x, PIN_COLOR1[n]);
  draw(x, black + linewidth(0.25pt));
  
  label(format("%02i", 40 - (n*2)), (dx0-0.125, n*dy0), align=E, white);
  
  //path x0 = shift(dx0, n * dy0) * pin0;
  //fill(x0, white);
  //draw(x0, black + linewidth(0.25pt));
}
