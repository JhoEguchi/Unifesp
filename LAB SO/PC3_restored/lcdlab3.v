module lcdlab3(
  input CLOCK_50,    //    50 MHz clock
  input KEY,      //    Pushbutton[3:0]
  input [16:0] SW,    //    Toggle Switch[17:0]
  input [1:0] Signal,
  input [4:0] Code,
  input [5:0] instrucao,
  output LCD_ON,    // LCD Power ON/OFF
  output LCD_BLON,    // LCD Back Light ON/OFF
  output LCD_RW,    // LCD Read/Write Select, 0 = Write, 1 = Read
  output LCD_EN,    // LCD Enable
  output LCD_RS,    // LCD Command/Data Select, 0 = Command, 1 = Data
  inout [7:0] LCD_DATA    // LCD Data bus 8 bits
);


wire [6:0] myclock;
wire RST;
assign RST = KEY;

// reset delay gives some time for peripherals to initialize
wire DLY_RST;
Reset_Delay r0(    .iCLK(CLOCK_50),.oRESET(DLY_RST) );

// turn LCD ON
assign    LCD_ON        =    1'b1;
assign    LCD_BLON    =    1'b1;

wire [3:0] hex1, hex0;

conversorbin TX(.binario(SW[6:0]), .dezena(hex1), .unidade(hex0));

LCD_Display u1(
// Host Side
   .iCLK_50MHZ(CLOCK_50),
   .iRST_N(DLY_RST),
   .hex0(hex0),
   .hex1(hex1),
	.Signal(Signal),
	.Code(Code),
	.instrucao(instrucao),
// LCD Side
   .DATA_BUS(LCD_DATA),
   .LCD_RW(LCD_RW),
   .LCD_E(LCD_EN),
   .LCD_RS(LCD_RS)
);


endmodule