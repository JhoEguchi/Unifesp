module beqorjump(a, b, flag);
	
	input a, b;
	output reg [1:0]flag;

	always @( a or b )
		begin
			flag = a | b;
		end

endmodule