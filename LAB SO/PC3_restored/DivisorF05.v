module DivisorF05(clk, s);

	input clk;
	output reg s;
	reg [25:0] cont;
	always @ (posedge clk) begin
	if(cont == 26'd900000)
	begin
		cont <= 26'd0;
		s <= 1;
	end
	else
	begin
		cont <= cont+1;
		s <= 0;
	end
end

endmodule