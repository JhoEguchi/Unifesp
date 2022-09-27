module BIOS
#(parameter DATA_WIDTH=32, parameter ADDR_WIDTH=10)
(
	input [(DATA_WIDTH-1):0] data,
	input [(ADDR_WIDTH-1):0] addr,
	input we, clk, clk50,
	output reg[(DATA_WIDTH-1):0] q
);

	// Declare the RAM variable
	reg [DATA_WIDTH-1:0] ram[2**ADDR_WIDTH-1:0];

	
	initial
	begin
	ram[0] = 32'b01001110001000000000000000000000;
	ram[1] = 32'b10001010001000000000000000000000;
	ram[2] = 32'b10000100000000000000000000000000;
	end
	
	always @ (negedge clk)
	begin
		// Write
		if (we)
			ram[addr] <= data;
	end

	always @ (negedge clk50)
	begin
		q <= ram[addr];	
	end

endmodule


//module BIOS (data, address, we, inclock, outclock, q);
//
//	input[31:0] data;
//	input[9:0] address;
//	input we, inclock, outclock;
//	output[31:0] q;
//
//	lpm_ram_dq boot (.data(data), .address(address), .we(we), .inclock(inclock), 
//                .outclock(outclock), .q(q));
//
//	defparam boot.lpm_width = 32;
//	defparam boot.lpm_widthad = 10;
//	defparam boot.lpm_indata = "REGISTERED";
//	defparam boot.lpm_outdata = "REGISTERED";
//	defparam boot.lpm_file = "bios.mif";
//
//endmodule