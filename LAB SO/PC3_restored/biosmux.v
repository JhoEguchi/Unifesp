module biosmux(clock ,reset ,resetPC ,biosInst ,memInst ,out);

	input clock, reset ;
	input [31:0] biosInst , memInst ;
	output [31:0] out ;
	output reg resetPC ;
	reg state ;
	
	initial
	begin
		state = 1'b0;
	end
	
	always @ (posedge clock) 
	begin
		if(reset) 
		begin
			state <= 1'b0;
			resetPC <= 1'b1;
		end
		else 
		begin
			if(state == 1'b0)
			begin
				if (biosInst[31:26] == 6'b100001)
				begin
					state <= 1'b1;
					resetPC <= 1'b1;
				end
				else
				begin
					resetPC <= 1'b0;
				end
			end
			else 
			begin
				resetPC <= 1'b0;
			end
		end
	end
	assign out = (state == 1'b0) ? biosInst : memInst ;
 endmodule
