// Quartus Prime Verilog Template
// Single port RAM with single read/write address 

module memoriadados 
#(parameter DATA_WIDTH=32, parameter ADDR_WIDTH=10)
(
	input [(DATA_WIDTH-1):0] data,
	input [(ADDR_WIDTH-1):0] addr,
	input we, clk, clk50,
	output reg[(DATA_WIDTH-1):0] q
);

	// Declare the RAM variable
	reg [DATA_WIDTH-1:0] ram[2**ADDR_WIDTH-1:0];

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


//module memoriadados (data, address, we, inclock, outclock, q);
//
//	input[31:0] data;
//	input[9:0] address;
//	input we, inclock, outclock;
//	output[31:0] q;
//
//	lpm_ram_dq datamemory (.data(data), .address(address), .we(we), .inclock(inclock), 
//                .outclock(outclock), .q(q));
//
//	defparam datamemory.lpm_width = 32;
//	defparam datamemory.lpm_widthad = 10;
//	defparam datamemory.lpm_indata = "REGISTERED";
//	defparam datamemory.lpm_outdata = "REGISTERED";
//
//endmodule
