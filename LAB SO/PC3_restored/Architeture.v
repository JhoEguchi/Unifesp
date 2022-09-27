module Architeture(clock50, switch, resetCPU, a, b, c, pushbutton, LCD_ON, LCD_BLON, LCD_RW, LCD_EN, LCD_RS, LCD_DATA);

	input resetCPU, clock50, pushbutton;
	input[16:0] switch;
	output[6:0] a, b, c;
	output LCD_ON, LCD_BLON, LCD_RW, LCD_EN, LCD_RS;
	inout[7:0] LCD_DATA;
	
	wire[31:0] datain, saida, BIOSInst, Code, instrucao;
	wire[9:0] addressIM;
	wire[11:0] address;
	wire[1:0] Signal;
	wire HDwe, clock;
	
	CPU CPU(.clock50(clock50), .clock(clock), .switch(switch), .resetCPU(resetCPU), .a(a), .b(b), .c(c), .pushbutton(pushbutton), .HDIndata(saida), .HDoutdata(datain), .HDwe(HDwe), .HDaddress(address), .BIOSInst(BIOSInst), .address(addressIM), .Signal(Signal), .saidajump(Code), .instrucao(instrucao));
	HD HD(.data(datain), .addr(address), .we(HDwe), .clk(clock), .clk50(clock50), .q(saida));
	lcdlab3 lcdlab3(.CLOCK_50(clock50), .KEY(pushbutton), .SW(switch), .Signal(Signal), .Code(Code[4:0]), .instrucao(instrucao[31:26]), .LCD_ON(LCD_ON), .LCD_BLON(LCD_BLON), .LCD_RW(LCD_RW), .LCD_EN(LCD_EN), .LCD_RS(LCD_RS), .LCD_DATA(LCD_DATA));
	BIOS BIOS (.data(0), .addr(addressIM[9:0]), .we(0), .clk(clock), .clk50(clock50), .q(BIOSInst));
	
endmodule